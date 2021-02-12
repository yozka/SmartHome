#pragma once

#include <Stream.h>
#include "sysHelper.h"
#include "trmParameters.h"


namespace Terminal
{
    ///--------------------------------------------------------------------------------------




    ///--------------------------------------------------------------------------------------
    namespace Settings
    {
        constexpr int lengthCommand = 100; //максимальное количество символов в команде
    }
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// Список команд, содержит в себе только одну команду
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    template<class ...TT>
    struct TCommands
    {
        template<class T>
        static bool executeCommand(const String &commandName, const AParameters &param, Stream *stream)
        {
            if (stream && commandName.equalsIgnoreCase(T::name()))
            {
                T::execute(param, stream);
                return true;
            }
            return false;
        };

        static bool execute(const String &commandName, const AParameters &param, Stream *stream)
        {
            return stream && ((executeCommand<TT>(commandName, param, stream)) || ... );
        };


        template<typename T>
        static void call(const T &functor) 
        { 
            ((functor(TT())), ...);
        };

     };
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// Терминал
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    template<class TCommands>
    class TTerminal
    {
        public:

            //проверка, соеденение
            bool isConnected(const int handle) const 
            {
                return handle == mConnectHandle;
            }

            //проверка, поток подключен к термиралу или нет
            bool isConnected(const Stream *stream) const 
            {
                return stream == mStream;
            }

            //подключение потока к терминалу
            int connect(Stream *stream, const bool echo)
            {
                if (mStream)
                {
                    disconnect();
                }
                mStream = stream;
                mEcho = echo;
                mStream->println(F("Connected OK"));
                commandInput();

                mConnectHandle++;
                return mConnectHandle;
            }



            //отключение от терминала
            void disconnect()
            {
                if (mStream)
                {
                    mStream->println(F("Disconnected..."));
                }
                reset();
            }



            //полное отключение от всего
            void reset()
            {
                mStream = nullptr;
                mCommandLine = "";
            }



            //обновление терминала, чтение данных из потока 
            //и формировние команд
            void update()
            {
                if (mStream && mStream->available() > 0)
                {
                    const char data = mStream->read();
                    switch (data)
                    {
                        case CR:
                        case LF: execute(); break;

                        case BS: backspace(); break;
        
                    default:
                        if (mCommandLine.length() < Settings::lengthCommand)
                        {
                            mCommandLine.concat(data);
                            if (mEcho)
                            {
                                mStream->write(data);
                            }
                        }
                        break;
                    }
                }
            }
            //


        private:

            int     mConnectHandle  = { 0 }; //соеденение
            bool    mEcho           = { true }; //дублирование ввода
            Stream* mStream         = { nullptr };
            String  mCommandLine;

        private:

            //выполнить команду
            void execute()
            {
                if (mEcho)
                {
                    mStream->println();
                }

                //получаем команду
                mCommandLine.trim();
                if (mCommandLine.length() == 0)
                {
                    //команды нет
                    if (mEcho)
                    {
                        commandInput();
                    }
                    return;
                }

                const int index = mCommandLine.indexOf(SPACE);
                const String name = (index < 0) ? mCommandLine : mCommandLine.substring(0, index);

                //создаем параметры
                AParameters param(index < 0 ? "" : mCommandLine.substring(index + 1));
                mCommandLine = "";

                //выполняем команду
                if (!TCommands::execute(name, param, mStream) && mStream)
                {
                    mStream->print(F(">>> Not found command: \""));
                    mStream->print(name);
                    mStream->println('\"');
                }

                commandInput();
            }

            //нажали затерание символа
            void backspace()
            {
                const auto len = mCommandLine.length();
                if (len > 0)
                {
                    mCommandLine.remove(len - 1);
                    if (mStream && mEcho)
                    {
                        mStream->write(BS);
                        mStream->write(SPACE);
                        mStream->write(BS);
                    }
                }
            }

            //начало ввода команды, сторка приглашения ввода
            void commandInput()
            {
                if (mStream)
                {
                    mStream->write('-');
                    mStream->write('>');
                }
                mCommandLine = "";
            }

    };/// TTerminal

}
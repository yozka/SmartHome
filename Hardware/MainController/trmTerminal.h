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
	/// Список команд, содержит в себе одну комануд и список следующих
	/// 
	/// 
	///--------------------------------------------------------------------------------------
    template<class T, class... TT>
    struct TCommands
    {
        using next = TCommands<TT...>; //следующая команда
        static bool execute(const String &commandName, const AParameters &param, Stream *stream)
        {
            if (commandName.equalsIgnoreCase(T::name()))
            {
                T cmd;
                cmd.execute(param, stream);
                return true;
            }
            return next::execute(commandName, param, stream);
        };
    };
	///--------------------------------------------------------------------------------------





     ///=====================================================================================
	///
	/// Список команд, содержит в себе только одну команду
	/// 
	/// 
	///--------------------------------------------------------------------------------------
    template<class T>
    struct TCommands<T>
    {
        static bool execute(const String &commandName, const AParameters &param, Stream *stream)
        {
            if (commandName.equalsIgnoreCase(T::name()))
            {
                T cmd;
                cmd.execute(param, stream);
                return true;
            }
            return false;
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

            //проверка, поток подключен к термиралу или нет
            bool isConnected(const Stream *stream = nullptr) const 
            {
                return (stream == nullptr) ? mStream != nullptr : stream == mStream;
            }

            //подключение потока к терминалу
            void connect(Stream *stream)
            {
                if (mStream)
                {
                    disconnect();
                }
                mStream = stream;
                mStream->println(F("Connected OK"));
                commandInput();
            }

            //отключение от терминала
            void disconnect()
            {
                if (mStream)
                {
                    mStream->println(F("Disconnected..."));
                    mStream = nullptr;
                }
                mCommandLine = "";
            }

            //обновление терминала, чтение данных из потока 
            //и формировние команд
            void update()
            {
                if (mStream && mStream->available())
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
                            mCommandLine += data;
                            mStream->write(data);
                        }
                        break;
                    }
                }
            }
            //


        private:

            Stream *mStream = { nullptr };
            String mCommandLine;

        private:

            //выполнить команду
            void execute()
            {
                //получаем команду
                mStream->println();
                mCommandLine.trim();
                if (mCommandLine.length() == 0)
                {
                    //команды нет
                    commandInput();
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
                if (mStream && len > 0)
                {
                    mStream->write(BS);
                    mStream->write(SPACE);
                    mStream->write(BS);
                    mCommandLine.remove(len - 1);
                }
            }

            //начало ввода команды, сторка приглашения ввода
            void commandInput()
            {
                if (mStream)
                {
                    mStream->write('>');
                }
                mCommandLine = "";
            }
 
    };/// TTerminal

}
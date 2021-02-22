#pragma once
#include <Arduino.h>
#include "../Network/network.h"
#include "../../user_config.h"

namespace Terminal
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Пробрасывает поток в терминал
    /// поток берется из сетевого интерфейса
    /// 
    ///--------------------------------------------------------------------------------------
    template <class TTerminal, class TSecurity>
    class TProviderServer
    {
        public:

            TProviderServer(TTerminal &terminal)
                :
                    mTerminal(terminal),
                    mSecurity(false)
            {

            }


            //настрйка соеденения
            void setup()
            {
                
            }

            //обновление и подключение к сетевому интерфейсу
            void update()
            {
                
                if ( auto guest = Network::ethernet.terminalGuestAccept(); guest != nullptr)
                {
                    //есть данные, но нет соеденение с терминалом
                    switch (mSecurity.process(guest))
                    {
                        case TSecurity::processing : break; //идет процесс аутентификации

                        case TSecurity::allow: 
                        {
                            //доступ разрешен
                            mTerminal.disconnect(); 
                            Network::ethernet.terminalGuestConnect();
                            mConnectHandle = mTerminal.connect(Network::ethernet.terminalClient(), false);
                            mTimeSession = millis() + Config::Network::Terminal::timeSession;
                            break; 
                        }

                        case TSecurity::deny:
                        {
                            //отсутствует доступ
                            Network::ethernet.terminalGuestDisconnect();
                            break; 
                        }  

                    }
                }

                statusDisconnected();
            }

        private:

            void statusDisconnected()
            {
                if (mConnectHandle == 0)
                {
                    return;
                }
                
                const bool terminalConnected    = mTerminal.isConnected(mConnectHandle);
                const bool timeSession          = mTimeSession > millis();

                if (auto client = Network::ethernet.terminalClient(); 
                                client &&                           //клиент живой
                                mTerminal.isConnected(client) &&    //подключен к терминалу корректно
                                terminalConnected &&                //хенд сессии корректен
                                timeSession)                        //время сессии невышла
                {
                    //клиент живой, под терминалом работает хорошо
                    return;
                }
                
                //проблема, клиент мертвый отключам все
                Network::ethernet.terminalClientDisconnect();
                if (terminalConnected)
                {
                    //клиент полностью отрублен, он не отвечает
                    mTerminal.reset();
                }

                mConnectHandle = 0;
                mTimeSession = 0;
            }

        private:

            TTerminal &mTerminal;
            TSecurity mSecurity;
            int mConnectHandle = { 0 }; //коннекция
            unsigned long mTimeSession = { 0 }; //время соеденения
        
    };

}
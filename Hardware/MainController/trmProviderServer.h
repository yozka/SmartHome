#pragma once
#include "Network.h"


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
                    mTerminal(terminal)
            {

            }


            //настрйка соеденения
            void setup()
            {
                
            }

            //обновление и подключение к сетевому интерфейсу
            void update()
            {
                
                if ( auto   guest = Network::ethernet.terminalGuestAvailable(); 
                            guest && !mTerminal.isConnected(guest))
                {
                    //есть данные, но нет соеденение с терминалом
                    switch (mSecurity.process(guest))
                    {
                        case TSecurity::processing : break; //идет процесс аутентификации

                        case TSecurity::allow: 
                        {
                            //доступ разрешен
                            mTerminal.connect(guest); 
                            Network::ethernet.terminalGuestConnect(guest);
                            break; 
                        }

                        case TSecurity::deny:
                        {
                            //отсутствует доступ
                            Network::ethernet.terminalGuestDisconnect(guest);
                            break; 
                        }  

                    }
                }

            }


        private:

            TTerminal &mTerminal;
            TSecurity mSecurity;
    };

}
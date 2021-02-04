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
                /*
                if ( auto client = mServer.available(); client == true && !mTerminal.isConnected(client) && client.available())
                {
                    //есть данные, но нет соеденение с терминалом
                    switch (mSecurity.process(client))
                    {
                        case TSecurity::allow      : mTerminal.connect(client); break; //доступ разрешен
                        case TSecurity::processing : break; //идет процесс аутентификации
                        case TSecurity::deny:
                        {
                            //отсутствует доступ
                            client.stop();
                            break; 
                        }  

                    }
                }
                */
            }


        private:

            TTerminal &mTerminal;
            TSecurity mSecurity;
    };

}
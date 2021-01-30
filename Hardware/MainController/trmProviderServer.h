#pragma once
#include "trmLogin.h"

namespace Terminal
{
	///--------------------------------------------------------------------------------------





     ///=====================================================================================
	///
	/// Пробрасывает поток в терминал
	/// поток берется из сетевого интерфейса
	/// 
	///--------------------------------------------------------------------------------------
    template <class TTerminal>
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

            }


        private:

            TTerminal &mTerminal;
            ALogin mLogin;
    };

}
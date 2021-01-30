#pragma once
#include <Arduino.h>

#include "trmLogin.h"

namespace Terminal
{
	///--------------------------------------------------------------------------------------





     ///=====================================================================================
	///
	/// Пробрасывает поток в терминал
	/// поток берется из последовтально порта
	/// 
	///--------------------------------------------------------------------------------------
    template <class TTerminal>
    class TProviderSerial
    {
        public:

            TProviderSerial(TTerminal &terminal)
                 :
                    mTerminal(terminal)
            {

            }
            

            //Настройка порта
            void setup()
            {
                Serial.begin(9600);
            }


            //подключение порта к терминалу
            void update()
            {
                if (!mTerminal.isConnected(&Serial) && Serial.available())
                {
                    //есть данные, но нет 
                    switch (mLogin.process(&Serial))
                    {
                        case ALogin::accept     : mTerminal.connect(&Serial); break; //доступ разрешен
                        case ALogin::deny       : break; //отсутствует доступ
                        case ALogin::processing : break; //идет процесс аутентификации
                    }

                }
            }
            //

        private:

            TTerminal &mTerminal;
            ALogin mLogin;
    };

}
#pragma once
#include <Arduino.h>


namespace Terminal
{
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// Пробрасывает поток в терминал
    /// поток берется из последовтально порта
    /// 
    ///--------------------------------------------------------------------------------------
    template <class TTerminal, class TSecurity>
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
                if (Serial.available() > 0 && !mTerminal.isConnected(&Serial))
                {
                    //есть данные, но нет соеденение с терминалом
                    switch (mSecurity.process(&Serial))
                    {
                        case TSecurity::allow      : mTerminal.connect(&Serial, true); break; //доступ разрешен
                        case TSecurity::deny       : break; //отсутствует доступ
                        case TSecurity::processing : break; //идет процесс аутентификации
                    }
                }
            }
            //

        private:

            TTerminal &mTerminal;
            TSecurity mSecurity;
    };

}
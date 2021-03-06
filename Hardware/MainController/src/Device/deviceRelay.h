#pragma once
#include <Arduino.h>



namespace Device
{
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// 
    /// Реле
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ARelay
    {
        public:
            using ptr = ARelay*;

        public:

            String name() const; //возвратим имя релюхи
            void setup(); //настройка релюхи

        public:

            //управление
            void turnOn();  //включить реле
            void turnOff(); //выключить реле

        protected:
            
            virtual String  onName() const = 0;     //получить имя релюхи
            virtual void    onSetup() = 0;          //общая настройка инциализация
            virtual void    onTurnOn() = 0;         //включить релюху
            virtual void    onTurnOff() = 0;        //тключить релюху

        private:

    };
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// 
    /// <summary>
    /// Реле
    /// привязанное непосредственно к контроллеру
    /// </summary>
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    template<uint8_t pin, typename meta>
    class TRelay
                    : public ARelay
    {
        protected:

            //получить имя релюхи
            String onName() const override
            {
                return meta::name();
            }

            //общая настройка инциализация
            void onSetup() override
            {
                pinMode(pin, OUTPUT); //перевели ножку контроллера на ввывод
            }

            //включить релюху
            void onTurnOn() override
            {
                digitalWrite(pin, HIGH); //переведем на ножку контроллера напряжение
            }

            //отключить релюху
            void onTurnOff() override
            {
                digitalWrite(pin, LOW);
            }
    };
    ///--------------------------------------------------------------------------------------








    ///--------------------------------------------------------------------------------------
}
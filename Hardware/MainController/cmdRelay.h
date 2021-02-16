#pragma once
#include <Arduino.h>
#include "trmParameters.h"
#include "user_config.h"

namespace Command
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Работа с релюхами
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ACommandRelay
    {
        public:

            static String name() { return F("Relay");}
            static void execute(const Terminal::AParameters &param, Stream *console);
            static void help(Stream *console);

        private:

            static void info    (Stream *console); //вывести информацию об релюхах
            static void turnOn  (Stream *console, const String &relayName); //включить реле
            static void turnOff (Stream *console, const String &relayName); //Выключить реле

            static Device::ARelay::ptr findRelay(Stream *console, const String &relayName); //поиск релюшки
    };
    ///--------------------------------------------------------------------------------------








    ///--------------------------------------------------------------------------------------
}
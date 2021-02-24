#pragma once
#include <Arduino.h>
#include "../Terminal/trmParameters.h"


namespace Command
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Работа с подсистемой связи MQTT
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ACommandMqtt
    {
        public:

            static String name() { return F("Mqtt");}
            static void execute(const Terminal::AParameters &param, Stream *console);
            static void help(Stream *console);

        private:

            static void info(Stream *console); //информация о протоколе
            static void configHelp(Stream *console); //информация о внутренних параметров для конфигурации
    };
    ///--------------------------------------------------------------------------------------








    ///--------------------------------------------------------------------------------------
}
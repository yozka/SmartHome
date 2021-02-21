#pragma once
#include <Arduino.h>
#include "trmParameters.h"


namespace Command
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Смена пароля и логина для терминала
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ACommandPasswd
    {
        public:

            static String name() { return F("Passwd");}
            static void execute(const Terminal::AParameters &param, Stream *console);
            static void help(Stream *console);

    };
    ///--------------------------------------------------------------------------------------








    ///--------------------------------------------------------------------------------------
}
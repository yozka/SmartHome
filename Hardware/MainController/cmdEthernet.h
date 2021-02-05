#pragma once
#include "trmParameters.h"


namespace Command
{
    ///--------------------------------------------------------------------------------------





    


     ///=====================================================================================
    ///
    /// Команда взаимодействия со сетевым интерфейсом
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ACommandIPConfig
    {
        public:

            static String name() { return F("ipconfig");}
            static void execute(const Terminal::AParameters &param, Stream *console);
            static void help(Stream *console);

        private:

            static void info(Stream *console); //вывести информацию об сетевом соеденении
            static void configHelp(Stream *console);
    };
    ///--------------------------------------------------------------------------------------

    







    ///--------------------------------------------------------------------------------------
}


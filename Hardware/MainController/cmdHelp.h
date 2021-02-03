#pragma once
#include <Arduino.h>
#include <Controllino.h>

#include "trmParameters.h"


namespace Command
{
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
    namespace Settings
    {
        constexpr int helpMargin = 12; //отступ команды
    }
    ///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// КОманда терминала, выводит помощь по всем командам
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class ACommandHelp
    {
        public:

            static String name() { return F("Help");}
            static void execute(const Terminal::AParameters &param, Stream &console);
            static void help(Stream &console)
            {
                console.println(F("-------------"));
            }

    };
    ///--------------------------------------------------------------------------------------









    ///--------------------------------------------------------------------------------------
    class TestCommandA
    {
        public:
            static String name() { return F("test");}
            static void help(Stream &console)
            {
                console.println(F("This is the test part of the program"));
            }
            static void execute(const Terminal::AParameters &param, Stream &console)
            {
                console.println(F("TEST!"));
            }
    };

    class TestCommandB
    {

        public:
            static String name() { return F("led");}
            static void help(Stream &console)
            {
                console.println(F("Turn on turn off control the LED"));
            }
            static void execute(const Terminal::AParameters &param, Stream &console)
            {
                if (param.source().equalsIgnoreCase(F("on")))
                {
                    digitalWrite(CONTROLLINO_D0, HIGH);
                    console.println(F("Led on"));
                } 
                else
                if (param.source().equalsIgnoreCase(F("off")))
                {
                    digitalWrite(CONTROLLINO_D0, LOW);
                    console.println(F("Led off"));
                }
                else
                {
                    console.println(F("led on|off"));
                }
            }
    };


    class TestCommandC
    {

        public:
            static String name() { return F("exit");}
            static void help(Stream &console) 
            {
                console.println(F("Test exit"));
            }
            static void execute(const Terminal::AParameters &param, Stream &console)
            {
                console.println(F("need exit.."));
            }
    };


    class TestCommandD
    {

        public:
            static String name() { return F("timer");}
            static void help(Stream &console)
            {
                console.println(F("Checking timer operation"));
            }
            static void execute(const Terminal::AParameters &param, Stream &console)
            {
                console.print(F("Start time test: "));
                const int timeDelay = param.source().toInt();
                console.println(timeDelay);
                const auto time = millis();
                delay(timeDelay);
                const auto timeEnd = millis() - time;
                console.print(F("Actual timer: "));
                console.println(timeEnd);
            }
    };
    ///--------------------------------------------------------------------------------------


    ///--------------------------------------------------------------------------------------
}
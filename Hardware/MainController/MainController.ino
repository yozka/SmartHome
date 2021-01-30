
/*
    по возможности не использовать STL
*/
#include <Arduino.h>

#include "trmSecurity.h"
#include "trmTerminal.h"
#include "trmProviderServer.h"
#include "trmProviderSerial.h"



class TestCommandA
{
    public:
        static String name() { return F("help");}
        void execute(const Terminal::AParameters &param, Stream *console)
        {
            console->println(F("NOT HELP!"));
        }
};

class TestCommandB
{

    public:
        static String name() { return F("led");}
        void execute(const Terminal::AParameters &param, Stream *console)
        {
            if (param.source().equalsIgnoreCase(F("on")))
            {
                digitalWrite(LED_BUILTIN, HIGH);
                console->println(F("Led on"));
            } 
            else
            if (param.source().equalsIgnoreCase(F("off")))
            {
                digitalWrite(LED_BUILTIN, LOW);
                console->println(F("Led off"));
            }
            else
            {
                console->println(F("led on|off"));
            }
        }
};


class TestCommandC
{

    public:
        static String name() { return F("exit");}
        void execute(const Terminal::AParameters &param, Stream *console)
        {
            console->println(F("need exit.."));
        }
};


class TestCommandD
{

    public:
        static String name() { return F("status");}
        void execute(const Terminal::AParameters &param, Stream *console)
        {
            console->println(F("--------"));

        }
};




//описаник команд
using ACommands = Terminal::TCommands<
                                        TestCommandA, 
                                        TestCommandB, 
                                        TestCommandC,
                                        TestCommandD>;

using ATerminal = Terminal::TTerminal<ACommands>;


//терминал для обработки внешних команд
ATerminal terminal;
Terminal::TProviderServer<ATerminal, Terminal::ASecurityFree>   providerServer(terminal);
Terminal::TProviderSerial<ATerminal, Terminal::ASecurityLogin>  providerSerial(terminal);




///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// настройка
/// 
/// 
///--------------------------------------------------------------------------------------
void setup() 
{
    providerSerial.setup();
    providerServer.setup();


    pinMode(LED_BUILTIN, OUTPUT);
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
void loop() 
{
        
    terminal.update();
    providerServer.update();
    providerSerial.update();
    
}

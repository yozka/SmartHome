
/*
    по возможности не использовать STL
*/
#include <Arduino.h>
#include <Controllino.h>



#include "trmSecurity.h"
#include "trmTerminal.h"
#include "trmProviderServer.h"
#include "trmProviderSerial.h"

#include "sysTimer.h"
///--------------------------------------------------------------------------------------



///--------------------------------------------------------------------------------------
class TestCommandA
{
    public:
        static String name() { return F("help");}
        static void execute(const Terminal::AParameters &param, Stream &console)
        {
            console.println(F("NOT HELP!"));
        }
};

class TestCommandB
{

    public:
        static String name() { return F("led");}
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
        static void execute(const Terminal::AParameters &param, Stream &console)
        {
            console.println(F("need exit.."));
        }
};


class TestCommandD
{

    public:
        static String name() { return F("timer");}
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





 ///=====================================================================================
///
/// Терминал контроллера
/// работает с ком портом и через сетевой интерфейс
/// 
///--------------------------------------------------------------------------------------

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




///--------------------------------------------------------------------------------------
Time::ASlowTimer slowTimer; //медленный таймер
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

    pinMode(CONTROLLINO_D0, OUTPUT);
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
    if (slowTimer.active())
    {
        //обработка терминала
        terminal.update();
        providerServer.update();
        providerSerial.update();
    }

    //обработка нажатия кнопок

    //обработка выводов
    //relay
    
}

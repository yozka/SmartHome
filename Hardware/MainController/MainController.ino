
/*
    по возможности не использовать STL
*/
#include <Arduino.h>



#include "user_config.h"
#include "sysTimer.h"
#include "netSystem.h"
#include "netMqtt.h"
#include "cmdTerminal.h"
#include "board.h"
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
    Board::setup();
    Board::relays.setup();  

    Network::ethernet.setup();
    Network::mqtt.setup();

    Terminal::providerServer.setup();
    Terminal::providerSerial.setup();
    
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
        Terminal::terminal.update();
        Terminal::providerServer.update();
        Terminal::providerSerial.update();
    }

    Network::mqtt.update();

    //обработка нажатия кнопок

    //обработка выводов
    //relay
    
    //Board::meta::Relay10::name();
    //Board::ARelay2::info::name();
}

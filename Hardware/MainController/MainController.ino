
/*
    по возможности не использовать STL
*/
#include <Arduino.h>



#include "user_config.h"
#include "src/Network/netSystem.h"
#include "src/Network/netMqtt.h"
#include "src/Command/cmdTerminal.h"
#include "src/Board/board.h"
#include "src/Systems/sysTimer.h"
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

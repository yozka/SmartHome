
/*
    по возможности не использовать STL
*/
#include <Arduino.h>



#include "user_config.h"
#include "src/Network/network.h"
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
    Board::buttons.setup();  

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

    //обработка входящий сообщений из сервера
    Network::mqtt.update();

    //обработка нажатия кнопок
    const auto time = millis();
    Board::buttons.update(time);


}

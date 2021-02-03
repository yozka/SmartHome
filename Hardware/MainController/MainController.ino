
/*
    по возможности не использовать STL
*/
#include <Arduino.h>
#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>


#include "sysTimer.h"
#include "commonTerminal.h"
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
    Controllino_RTC_init();


    // the media access control (ethernet hardware) address for the shield:
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    //the IP address for the shield:
    byte ip[] = { 10, 0, 0, 177 };    
    // the router's gateway address:
    byte gateway[] = { 10, 0, 0, 1 };
    // the subnet:
    byte subnet[] = { 255, 255, 0, 0 };

    Ethernet.begin(mac, ip, gateway, subnet);


    Common::providerServer.setup();
    Common::providerSerial.setup();
    

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
        Common::terminal.update();
        Common::providerServer.update();
        Common::providerSerial.update();
    }

    //обработка нажатия кнопок

    //обработка выводов
    //relay
    
}

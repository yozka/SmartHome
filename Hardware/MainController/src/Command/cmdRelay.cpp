#include "cmdRelay.h"
#include "../Board/board.h"



using namespace Command;
///--------------------------------------------------------------------------------------







 ///=====================================================================================
///
/// Команда вывод информации по внутренней памяти девайса
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandRelay::help(Stream *console)
 {
      console->println(F("Working with relays"));
 }
///--------------------------------------------------------------------------------------






void ACommandRelay::execute(const Terminal::AParameters &param, Stream *console)
{
    if (param.isEmpty())
    {
        info(console);
        return;
    }

    //проверим на наличие команды по включению выключению реле
    const auto turn = param.queryIndex(1);
    if (turn.equalsIgnoreCase(F("on")))
    {
        turnOn(console, param.queryIndex(0));
        return;
    }
    if (turn.equalsIgnoreCase(F("off")))
    {
        turnOff(console, param.queryIndex(0));
        return;
    }


    //выведем информацию что ввели неправельные параметры
    console->print(F("Invalid parameters: \""));
    console->print(param.source());
    console->println('"');
    console->println(turn);
}




//вывести информацию об релюхах
void ACommandRelay::info(Stream *console)
{
    for (const auto relay : Board::relays.data)
    {
        console->println(relay->name());
    }
}



//поиск релюшки
Device::ARelay::ptr ACommandRelay::findRelay(Stream *console, const String &relayName)
{
    if (auto relay = Board::relays.findDevice(relayName))
    {
        return relay;
    }

    console->print(F("Relay not found - "));
    console->println(relayName);
    return {};
}


//включить реле
void ACommandRelay::turnOn(Stream *console, const String &relayName)
{
    if (auto relay = findRelay(console, relayName))
    {
        console->print(F("Turn ON "));
        console->println(relayName);
        relay->turnOn();
    }
}


//Выключить реле
void ACommandRelay::turnOff(Stream *console, const String &relayName)
{
    if (auto relay = findRelay(console, relayName))
    {
        console->print(F("Turn OFF "));
        console->println(relayName);
        relay->turnOff();
    }
}

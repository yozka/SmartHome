#include "cmdMqtt.h"
#include "../Network/netMqtt.h"
#include "../Systems/sysStorage.h"
#include "../Systems/sysHash.h"
///--------------------------------------------------------------------------------------




///--------------------------------------------------------------------------------------
using namespace Command;
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// 
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandMqtt::help(Stream *console)
 {
      console->println(F("Mqtt client protocol"));
 }
///--------------------------------------------------------------------------------------






void ACommandMqtt::execute(const Terminal::AParameters &param, Stream *console)
{
    if (param.isEmpty())
    {
        info(console);
        return;
    }

     if (param.isKey('?'))
     {
          configHelp(console);
          return;
     }

    //устанавливаемый параметр
    const String cmd = param[0];
    
    //установка индификатор клиента
    if (cmd.equalsIgnoreCase(F("clientid")))
    {
        console->println(F("OK"));
        Network::mqtt.setClientId(param[1]);
        info(console);
        return;
    }

    //установка логин ипароль
    if (cmd.equalsIgnoreCase(F("login")))
    {
        console->println(F("OK"));
        Network::mqtt.setUsernamePassword(param[1], param[2]);
        info(console);
        return;
    }

    //установка адреса сервера с портом
    if (cmd.equalsIgnoreCase(F("broker")))
    {
        console->println(F("OK"));
        Network::mqtt.setBroker(param[1]);
        info(console);
        return;
    }

    console->print(F("Unknown parameter: "));
    console->println(cmd);
    configHelp(console);
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// 
/// информация о протоколе
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandMqtt::info(Stream *console)
 {
    //cтатус активности, есть соеденение или нет
    //если нет, то показать ошибку коннекта
    //имя клиента
    //имя логина
    //пароль если есть
    //адрес брокера
    //порт брокера

    console->println(F("MQTT client protocol"));
    //
    console->print(F("Connect status: "));
    if (Network::mqtt.isConnected())
    {
        console->println(F("CONNECTED"));
    }
    else
    {
        console->print(F("Error - "));
        console->println(Network::mqtt.error());
    }
    //

    console->print(F("     Client id: ")); console->println(Network::mqtt.clientId());
    console->print(F("         Login: ")); console->println(Network::mqtt.login());
    console->print(F("      Password: ")); console->println(Network::mqtt.passwd().length() ? F("*****") : F(""));
    console->print(F("        Broker: ")); console->println(Network::mqtt.broker());
    console->print(F("          Port: ")); console->println(Network::mqtt.port());


 }
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// 
/// вывести справку по параметрам
/// 
/// 
///--------------------------------------------------------------------------------------
void ACommandMqtt::configHelp(Stream *console)
{
    
     console->println(F("MQTT config parameters:"));
     console->println(F("mqtt clientid <client_id>"));
     console->println(F("mqtt login [username] [password]"));
     console->println(F("mqtt broker <address>[:port]"));
}
///--------------------------------------------------------------------------------------



#include "cmdEthernet.h"
#include "../Network/network.h"



using namespace Command;
///--------------------------------------------------------------------------------------







 ///=====================================================================================
///
/// Команда настройки даты на контроллере
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandIPConfig::help(Stream *console)
 {
      console->println(F("Configuring IP Protocol [ipconfig /?]"));
 }



void ACommandIPConfig::execute(const Terminal::AParameters &param, Stream *console)
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

     if (param.source().equalsIgnoreCase(F("restore")))
     {
          Network::ethernet.restore();
          info(console);
          return;
     }


     bool error = false; 

     //Мак адрес
     if (const String data = param.queryValue(F("mac")); data.length() > 0)
     {
          error |= !Network::ethernet.setMACAddress(data);
          console->print(F("MAC: ")); console->println(Network::ethernet.MACAddress());
     }
   
     //айпиадрес
     if (const String data = param.queryValue(F("ip")); data.length() > 0)
     {
          error |= !Network::ethernet.setLocalIP(data);
          console->print(F("IPv4: ")); console->println(Network::ethernet.localIP());
     }

     //маска подсети
     if (const String data = param.queryValue(F("mask")); data.length() > 0)
     {
          error |= !Network::ethernet.setSubnetMask(data);
          console->print(F("Subnet mask: ")); console->println(Network::ethernet.subnetMask());
     }

     //основной шлюз сервера
     if (const String data = param.queryValue(F("gateway")); data.length() > 0)
     {
          error |= !Network::ethernet.setGatewayIP(data);
          console->print(F("Gateway: ")); console->println(Network::ethernet.gatewayIP());
     }

     //днс сервер
     if (const String data = param.queryValue(F("dns")); data.length() > 0)
     {
          error |= !Network::ethernet.setDnsServerIP(data);
          console->print(F("DNS server: ")); console->println(Network::ethernet.dnsServerIP());
     }

     //Ошибка конфигурации
     if (error)
     {
          console->println(F("Config parametrs ERROR"));
     }
}
///--------------------------------------------------------------------------------------





//вывести информацию об сетевом соеденении
void ACommandIPConfig::info(Stream *console)
{
     console->print(F("    Chipset: ")); console->println(Network::ethernet.chipset());
     console->print(F("        MAC: ")); console->println(Network::ethernet.MACAddress());
     console->print(F("       IPv4: ")); console->println(Network::ethernet.localIP());
     console->print(F("Subnet mask: ")); console->println(Network::ethernet.subnetMask());
     console->print(F("    Gateway: ")); console->println(Network::ethernet.gatewayIP());
     console->print(F(" DNS server: ")); console->println(Network::ethernet.dnsServerIP());
}


//вывести справку по параметрам
void ACommandIPConfig::configHelp(Stream *console)
{
     console->println(F("Ethernet config parameters:"));
     console->print(F("ipconfig mac "));      console->println(Network::ethernet.MACAddress());
     console->print(F("ipconfig ip "));       console->println(Network::ethernet.localIP());
     console->print(F("ipconfig mask "));     console->println(Network::ethernet.subnetMask());
     console->print(F("ipconfig gateway "));  console->println(Network::ethernet.gatewayIP());
     console->print(F("ipconfig dns "));      console->println(Network::ethernet.dnsServerIP());
     
     console->println();
     console->println(F("Restore default settings:")); 
     console->println(F("ipconfig restore")); 
}



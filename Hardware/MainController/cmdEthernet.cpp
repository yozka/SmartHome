#include "cmdEthernet.h"
#include "network.h"



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

     const auto data = param.source();
     if (data.indexOf('?') >= 0)
     {
          configHelp(console);
          return;
     }


   
}
///--------------------------------------------------------------------------------------





//вывести информацию об сетевом соеденении
void ACommandIPConfig::info(Stream *console)
{
     console->print(F("   Ethernet: ")); console->println(Network::ethernet.link() ? F("Link ON") : F("Link OFF"));
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
}

#include "Network.h"
#include "sysHelper.h"


#include <SPI.h>
#include <Ethernet.h>



namespace Network
{
    AEthernet ethernet;
}


namespace Local
{
    EthernetServer terminal_server = EthernetServer(Network::Settings::port);
    EthernetClient terminal_client = EthernetClient(MAX_SOCK_NUM); //активный клиент
    EthernetClient terminal_linked = EthernetClient(MAX_SOCK_NUM); //клиент который подкючается
}



//настройка сетевого интерфейса
void Network::AEthernet::setup()
{

    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    byte dns[] = { 8, 8, 8, 8 };    
    byte ip[] = { 192, 168, 1, 160 };    
    byte gateway[] = { 192, 168, 1, 1 };
    byte subnet[] = { 255, 255, 0, 0 };


    Ethernet.begin(mac, ip, dns, gateway, subnet);

    Local::terminal_server = EthernetServer(33);
}





String Network::AEthernet::MACAddress() const
{
    String mac;
    byte macBuffer[6];  // create a buffer to hold the MAC address
    Ethernet.MACAddress(macBuffer); 
    for (byte octet = 0; octet < 6; octet++) 
    {
        mac += arrayToHex(macBuffer + octet, 1);
        if (octet < 5) 
        {
            mac += '-';
        }
    }
    return mac;
}




String IPAddressToString(const IPAddress &address)
{
    String s;
    s += address[0]; s += '.';
    s += address[1]; s += '.';
    s += address[2]; s += '.';
    s += address[3]; 
    return s;
}




String Network::AEthernet::localIP() const
{
    return IPAddressToString(Ethernet.localIP());
}




String Network::AEthernet::subnetMask() const
{
    return IPAddressToString(Ethernet.subnetMask());
}




String Network::AEthernet::gatewayIP() const
{
    return IPAddressToString(Ethernet.gatewayIP());
}




String Network::AEthernet::dnsServerIP() const
{
    return IPAddressToString(Ethernet.dnsServerIP());
}



//есть или нет соеденение 
bool Network::AEthernet::link() const
{
    return Ethernet.linkStatus() == LinkON;
}




//возваритим подключенного терминал клиента
Stream& Network::AEthernet::terminalAvailable()
 {
     //return Local::terminal_server.available();
     //return Local::terminal_client;
     //auto test =  EthernetClient(MAX_SOCK_NUM);
     return Local::terminal_client;
 }
#include "Network.h"
#include "sysHelper.h"


#include <SPI.h>
#include <Ethernet.h>

//ArduinoMqttClient by Arduino Version 0.1.5 INSTALLED


///--------------------------------------------------------------------------------------
namespace Network
{
    AEthernet ethernet;



    ///----------------------------------------------------------------------------------
    namespace Terminal
    {
        EthernetServer server   = EthernetServer(Network::Settings::port); //сам сервер
        EthernetClient client   = EthernetClient(MAX_SOCK_NUM); //активный клиент который подключен
        EthernetClient guest    = EthernetClient(MAX_SOCK_NUM); //готсь в режиме подключения
    }
}
///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// настройка сетевого интерфеса
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::setup()
    {

        byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
        byte dns[] = { 8, 8, 8, 8 };    
        byte ip[] = { 192, 168, 1, 160 };    
        byte gateway[] = { 192, 168, 1, 1 };
        byte subnet[] = { 255, 255, 0, 0 };


        Ethernet.begin(mac, ip, dns, gateway, subnet);

        Network::Terminal::server = EthernetServer(33);
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



    //установка макадреса
    bool Network::AEthernet::setMACAddress(const String &mac)
    {
        if (mac.length() != 17)
        {
            return false;
        }

        byte macBuffer[6];
        for (int i = 0; i < 6; i++)
        {
            const int idx = i * 3;
            macBuffer[i] = hexToDec(mac.substring(idx, idx + 2));
        }
        Ethernet.setMACAddress(macBuffer); 
        return true;
    }



    //установка локлаьного адреса
    bool Network::AEthernet::setLocalIP(const String &address)
    {
        IPAddress ip;
        if (ip.fromString(address))
        {
            Ethernet.setLocalIP(ip);
            return true;
        }
        return false;
    }



    //Установка маски подсети
    bool Network::AEthernet::setSubnetMask(const String &mask)
    {
        IPAddress ip;
        if (ip.fromString(mask))
        {
            Ethernet.setSubnetMask(ip);
            return true;
        }
        return false;
    }



    //установка шлюза сервера
    bool Network::AEthernet::setGatewayIP(const String &address)
    {
        IPAddress ip;
        if (ip.fromString(address))
        {
            Ethernet.setGatewayIP(ip);
            return true;
        }
        return false;
    }



    //установка альтернативного DNS
    bool Network::AEthernet::setDnsServerIP(const String &address)
    {
        IPAddress ip;
        if (ip.fromString(address))
        {
            Ethernet.setDnsServerIP(ip);
            return true;
        }
        return false;
    }
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Система сетевого взаимодействия
    /// 
    /// возваритим подключенного терминал клиента
    /// 
    ///--------------------------------------------------------------------------------------
    Stream* Network::AEthernet::terminalGuestAvailable()
    {
        auto local = Network::Terminal::server.available();
        if (!local)
        {
            //отсутствует какоелибо соеденение
            return nullptr;
        }

        if (!Network::Terminal::guest)
        {
            //пришло новое седенение
            Network::Terminal::guest = local;
        }
        else
        if (Network::Terminal::guest != local)
        {
            //занят доступ
            return nullptr;
        }

        return &Network::Terminal::guest;
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// подключим гостя в систему
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::terminalGuestConnect( Stream *guest )
    {
         if (&Network::Terminal::guest == guest)
         {
             Network::Terminal::client.stop();
             Network::Terminal::client = Network::Terminal::guest;
             Network::Terminal::guest = {};
         }
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// отключаем готстя
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::terminalGuestDisconnect( Stream *guest )
    {
        if (&Network::Terminal::guest == guest)
        {
            Network::Terminal::guest.stop();
            Network::Terminal::guest = {};
        }
    }


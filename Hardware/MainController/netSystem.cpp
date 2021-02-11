#include "netSystem.h"
#include "sysHelper.h"
#include "configuration.h"


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
        EthernetServer server = EthernetServer(Configuration::Network::Terminal::port); //сам сервер
        EthernetClient client; //активный клиент который подключен
        EthernetClient guest; //готсь в режиме подключения
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
        /*
        byte mac[];{Configuration::Network::mac};
        byte dns[]{Configuration::Network::dns};
        byte ip[]{Configuration::Network::ip};
        byte gateway[]{Configuration::Network::gateway};
        byte subnet[]{Configuration::Network::subnet};*/

        byte mac[]        = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
        byte dns[]        = { 192, 168, 0, 1 };    
        byte ip[]         = { 192, 168, 0, 160 };    
        byte gateway[]    = { 192, 168, 0, 1 };
        byte subnet[]     = { 255, 255, 255, 0 };

        Ethernet.begin(mac, ip, dns, gateway, subnet);

        Network::Terminal::server.begin();
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
    String Network::AEthernet::chipset() const
    {/*
        switch (Ethernet.hardwareStatus())
        {
            case EthernetNoHardware :   return F("none");
            case EthernetW5100 :        return F("W5100");
            case EthernetW5200 :        return F("W5200");
            case EthernetW5500 :        return F("W5500");
        default:
            break;            
        }
        return F("Unknown");*/
        return {};
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
    Stream* Network::AEthernet::terminalGuestAccept()
    {
        auto local = Network::Terminal::server.accept();
        if (Network::Terminal::guest.connected())
        {
            //гость подключен, все отсальные подключение разрывать
            local.stop();
            return &Network::Terminal::guest;
        }

        if (local)
        {
            //есть новое подключение
            Network::Terminal::guest = local;
            return &Network::Terminal::guest;
        }

        return nullptr;
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// подключим гостя в систему
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::terminalGuestConnect()
    {
        terminalClientDisconnect();
        Network::Terminal::client = Network::Terminal::guest;
        Network::Terminal::guest = {};
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// отключаем готстя
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::terminalGuestDisconnect()
    {
        Network::Terminal::guest.stop();
        Network::Terminal::guest = {};
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// получим подключенного клиента
    /// 
    ///--------------------------------------------------------------------------------------
    Stream* Network::AEthernet::terminalClient()
    {
        if (Network::Terminal::client.connected())
        {
            return &Network::Terminal::client;
        }
        return nullptr;
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// отключим клиента полностью
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AEthernet::terminalClientDisconnect()
    {
        Network::Terminal::client.stop();
    }
    ///--------------------------------------------------------------------------------------




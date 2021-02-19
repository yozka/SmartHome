#include "netSystem.h"
#include "sysUtils.h"
#include "sysStorage.h"
#include "user_config.h"
#include "sysHash.h"


#include <SPI.h>
#include <Ethernet.h>




///--------------------------------------------------------------------------------------
namespace Network
{
    AEthernet ethernet;



    ///----------------------------------------------------------------------------------
    namespace Terminal
    {
        EthernetServer server = EthernetServer(Config::Network::Terminal::port); //сам сервер
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
 
        byte mac[]          = { 0xAD, 0xBD, 0xBE, 0xEF, 0xFE, 0xED };  
        IPAddress dns       = { 192, 168, 0, 1 };
        IPAddress ip        = { 192, 168, 0, 160 };
        IPAddress gateway   = { 192, 168, 0, 1 };
        IPAddress subnet    = { 255, 255, 255, 0 };

        sys::AStorage storage;

        storage.read(sys::hash_const("mac"), mac);
        dns =       storage.read_uint32(sys::hash_const("dns"), dns);
        ip =        storage.read_uint32(sys::hash_const("ip"), ip);
        gateway =   storage.read_uint32(sys::hash_const("gateway"), gateway);
        subnet =    storage.read_uint32(sys::hash_const("subnet"), subnet);

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
            mac += sys::arrayToHex(macBuffer + octet, 1);
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
    {
        switch (Ethernet.hardwareStatus())
        {
            case EthernetNoHardware :   return F("none");
            case EthernetW5100 :        return F("W5100");
            case EthernetW5200 :        return F("W5200");
            case EthernetW5500 :        return F("W5500");
        default:
            break;            
        }
        return F("Unknown");
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
            macBuffer[i] = sys::hexToDec(mac.substring(idx, idx + 2));
        }

        sys::AStorage storage;
        storage.write(sys::hash_const("mac"), macBuffer);
        Ethernet.setMACAddress(macBuffer); 
        return true;
    }



    //установка локлаьного адреса
    bool Network::AEthernet::setLocalIP(const String &address)
    {
        IPAddress ip;
        if (ip.fromString(address))
        {
            sys::AStorage storage;
            storage.write_uint32(sys::hash_const("ip"), ip);
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
            sys::AStorage storage;
            storage.write_uint32(sys::hash_const("subnet"), ip);
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
            sys::AStorage storage;
            storage.write_uint32(sys::hash_const("gateway"), ip);
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
            sys::AStorage storage;
            storage.write_uint32(sys::hash_const("dns"), ip);
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




#pragma once
#include <Arduino.h>
#include <Stream.h>

namespace Network
{
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
    namespace Settings
    {
        constexpr uint16_t port = 23; //порт подключения к устройству
    }
    ///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// Система сетевого взаимодействия
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class AEthernet
    {
        public:

            void setup(); //настройка сетевого интерфейса

        public:

            bool link() const; //есть или нет соеденение 

            String MACAddress() const;
            String localIP() const;
            String subnetMask() const;
            String gatewayIP() const;
            String dnsServerIP() const;

            bool setMACAddress  (const String &mac); //установка макадреса
            bool setLocalIP     (const String &address); //установка локлаьного адреса
            bool setSubnetMask  (const String &mask); //Установка маски подсети
            bool setGatewayIP   (const String &address); //установка шлюза сервера
            bool setDnsServerIP (const String &address); //установка альтернативного DNS

        public:


            //сетевые данные для терминала
            Stream* terminalGuestAvailable(); //возваритим подключенного терминал клиента
            void terminalGuestConnect( Stream *guest ); //подключим гостя в систему
            void terminalGuestDisconnect( Stream *guest); //отключаем готстя
    };
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
    extern AEthernet ethernet;
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
}
#pragma once
#include <Arduino.h>
#include <Stream.h>

namespace Network
{
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
            void restore(); //востановить настройки по умолчанию, и перезагрузить сетевое окружение
        public:

            String chipset() const; //возвратим тип чипа

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
            Stream* terminalGuestAccept(); //возваритим подключенного терминал клиента
            void terminalGuestConnect(); //подключим гостя в систему как клиента
            void terminalGuestDisconnect(); //отключаем готстя

            Stream* terminalClient();//получим подключенного клиента
            void terminalClientDisconnect();//отключим клиента полностью

        private:
            
            void ethernetSetup(); //начальная загрузка параметров
    };
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
    extern AEthernet ethernet;
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
}
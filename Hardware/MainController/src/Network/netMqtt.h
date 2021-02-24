#pragma once
#include <Arduino.h>

namespace Network
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Система сетевого протокола
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class AMqtt
    {
        public:

            void setup(); //настройка сетевого интерфейса
            void update();
           
        public:

            bool isConnected() const; //возвратим, есть связь или нет
            String error() const; //возвратим ошибку ввиде строчки

            String clientId() const;
            String login() const;
            String passwd() const;
            String broker() const;
            uint16_t port() const;

            void setClientId(const String &clientId); //установка индификатора протокола
            void setUsernamePassword(const String &login, const String &passwd); //установка логин и пароль
            void setBroker(const String &brokerAddressPort); //установка брокера совместно с адресом
        private:

            void reconnect(); //делам переподключение
            bool connect();//подключаемся
            void onMqttMessage(int messageSize); //приходящее сообщение

        private:

            bool mLastConnect               = { false };
            unsigned long mTimeReconnect    = { 0 }; //время переподключение к серверу
            unsigned long mTimeStep         = { 0 }; //время для следющей попытки переподключения

    };
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
    extern AMqtt mqtt;
    ///--------------------------------------------------------------------------------------



    ///--------------------------------------------------------------------------------------
}
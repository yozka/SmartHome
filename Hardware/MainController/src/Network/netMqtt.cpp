#include "netMqtt.h"

#include "../Board/board.h"
#include "../Systems/sysUtils.h"
#include "../Systems/sysStorage.h"
#include "../Systems/sysHash.h"
#include "../../user_config.h"
///--------------------------------------------------------------------------------------




///--------------------------------------------------------------------------------------
#include <Ethernet.h>
#include <ArduinoMqttClient.h>
///--------------------------------------------------------------------------------------





///--------------------------------------------------------------------------------------
namespace Network
{
    AMqtt mqtt;



    ///----------------------------------------------------------------------------------
    namespace Mqtt
    {
        //само соедениение со сервером, и клиент протокола, 
        //это глобальныая переменная
        EthernetClient tcp;
        MqttClient client(tcp);
    }
}
///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// настройка протокола
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::setup()
    {


        Network::Mqtt::client.onMessage([](int messageSize)
        {
            Network::mqtt.onMqttMessage(messageSize);
        });

    }
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// 
    /// Обновление протокола
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::update()
    {
        if (Network::Mqtt::client.connected())
        {
            Network::Mqtt::client.poll();
            return;
        }

        //нет связи, делаем переподключение
        const auto time = millis();
        if (time > mTimeReconnect || mLastConnect)
        {
            mTimeReconnect = min(Config::Network::Mqtt::timeReconnect + mTimeStep, Config::Network::Mqtt::timeMax);
            mTimeReconnect += time;
            mTimeStep += Config::Network::Mqtt::timeStep;

            mLastConnect = connect();
            if (mLastConnect)
            {
                //было успешное подключение, сбросим таймер
                mTimeStep = 0;
            }
        }
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// переподключение к серверу
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::reconnect()
    {
        if (Network::Mqtt::client.connected())
        {
            Network::Mqtt::tcp.stop();
            Network::Mqtt::client.stop();
        }
        mLastConnect = connect();
        if (mLastConnect)
        {
            mTimeStep = 0;
        }
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// переподключение к серверу
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    bool Network::AMqtt::connect()
    {
        sys::AStorage storage;


        Network::Mqtt::client.setId(storage.readString(sys::hash_const("mqtt-clientId")));
        Network::Mqtt::client.setUsernamePassword(storage.readString(sys::hash_const("mqtt-login")),
                                                  storage.readString(sys::hash_const("mqtt-passwd")));

    
        const String broker = storage.readString(sys::hash_const("mqtt-broker"));
        const uint16_t port = storage.read_uint16(sys::hash_const("mqtt-port"), Config::Network::Mqtt::port);
        if (!Network::Mqtt::client.connect(broker.c_str(), port)) 
        {
            //проблема со связью
            //соеденения нет
            return false;
        }

        // subscribe to a topic
        // the second parameter set's the QoS of the subscription,
        // the the library supports subscribing at QoS 0, 1, or 2
        int subscribeQos = 1;
        String inTopic = F("base/relay/led1");
        Network::Mqtt::client.subscribe(inTopic, subscribeQos);

        return true;
    }




     ///=====================================================================================
    ///
    /// приходящее сообщение
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::onMqttMessage(int messageSize)
    {
        /*
        
        // we received a message, print out the topic and contents
        Serial.print("Received a message with topic '");
        Serial.print(Network::Mqtt::client.messageTopic());
        Serial.print("', duplicate = ");
        Serial.print(Network::Mqtt::client.messageDup() ? "true" : "false");
        Serial.print(", QoS = ");
        Serial.print(Network::Mqtt::client.messageQoS());
        Serial.print(", retained = ");
        Serial.print(Network::Mqtt::client.messageRetain() ? "true" : "false");
        Serial.print("', length ");
        Serial.print(messageSize);
        Serial.println(" bytes:");

        // use the Stream interface to print the contents
        while (Network::Mqtt::client.available()) {
            Serial.print((char)Network::Mqtt::client.read());
        }
        Serial.println();

        Serial.println();*/

        int turn = 0;

        while (Network::Mqtt::client.available())
        {
            auto data = Network::Mqtt::client.read();
            if (data == '1')
            {
               turn = 11;
            }
            if (data == '0')
            {
                turn = 10;
            }

        }

        if (turn == 11)
        {
            Board::relays.data[0]->turnOn();
        }
        if (turn == 10)
        {
            Board::relays.data[0]->turnOff();
        }


    }
    ///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// возвратим, есть связь или нет
    /// 
    ///--------------------------------------------------------------------------------------
    bool Network::AMqtt::isConnected() const
    {
        return Network::Mqtt::client.connected();
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// возвратим ошибку ввиде строчки
    /// 
    ///--------------------------------------------------------------------------------------
    String Network::AMqtt::error() const
    {
        const auto code = Network::Mqtt::client.connectError();
        switch (code)
        {
            case MQTT_CONNECTION_REFUSED            : return F("Connection refused");
            case MQTT_CONNECTION_TIMEOUT            : return F("Connection timeout");
            case MQTT_SUCCESS                       : return F("Success");
            case MQTT_UNACCEPTABLE_PROTOCOL_VERSION : return F("Unacceptable protocol version");
            case MQTT_IDENTIFIER_REJECTED           : return F("Identifier rejected");
            case MQTT_SERVER_UNAVAILABLE            : return F("Server unavailable");
            case MQTT_BAD_USER_NAME_OR_PASSWORD     : return F("Bad user name or password");
            case MQTT_NOT_AUTHORIZED                : return F("Not authorized");
        }
        return String(code);
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    String Network::AMqtt::clientId() const
    {
        sys::AStorage storage;
        return storage.readString(sys::hash_const("mqtt-clientId"));
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::setClientId(const String &clientId)
    {
        sys::AStorage storage;
        storage.writeString(sys::hash_const("mqtt-clientId"), clientId);
        reconnect();
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    String Network::AMqtt::login() const
    {
        sys::AStorage storage;
        return storage.readString(sys::hash_const("mqtt-login"));
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    String Network::AMqtt::passwd() const
    {
        sys::AStorage storage;
        return storage.readString(sys::hash_const("mqtt-passwd"));
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// установка логин и пароль
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::setUsernamePassword(const String &login, const String &passwd)
    {
        sys::AStorage storage;
        storage.writeString(sys::hash_const("mqtt-login"), login);
        storage.writeString(sys::hash_const("mqtt-passwd"), passwd);
        reconnect();
    }
    ///--------------------------------------------------------------------------------------
    



     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    String Network::AMqtt::broker() const
    {
        sys::AStorage storage;
        return storage.readString(sys::hash_const("mqtt-broker"));
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    uint16_t Network::AMqtt::port() const
    {
        sys::AStorage storage;
        return storage.read_uint16(sys::hash_const("mqtt-port"), Config::Network::Mqtt::port);
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// установка брокера совместно с адресом
    /// 
    ///--------------------------------------------------------------------------------------
    void Network::AMqtt::setBroker(const String &brokerAddressPort)
    {
        sys::AStorage storage;

        String broker = brokerAddressPort;
        uint16_t port = storage.read_uint16(sys::hash_const("mqtt-port"), Config::Network::Mqtt::port);

        if (const auto idx = brokerAddressPort.lastIndexOf(':'); idx > 0)
        {
            //в адресе предположительно есть порт
            const auto iPort = brokerAddressPort.substring(idx + 1).toInt();
            if (iPort > 0)
            {
                port = iPort;
                broker = brokerAddressPort.substring(0, idx);
            }
        }

        //
        storage.writeString(sys::hash_const("mqtt-broker"), broker);
        storage.write_uint16(sys::hash_const("mqtt-port"), port);
        reconnect();
    }
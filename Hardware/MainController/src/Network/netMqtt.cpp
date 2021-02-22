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
            mTimeReconnect = time + Config::Network::Mqtt::timeReconnect;
            mLastConnect = reconnect();
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
    bool Network::AMqtt::reconnect()
    {
        sys::AStorage storage;



        Network::Mqtt::client.setId(storage.readString(sys::hash_const("mqtt-clientId")));
        Network::Mqtt::client.setUsernamePassword(storage.readString(sys::hash_const("mqtt-login")),
                                                  storage.readString(sys::hash_const("mqtt-passwd")));

    
        const String broker = storage.readString(sys::hash_const("mqtt-broker"));
        const uint16_t port = storage.read_uint16(sys::hash_const("mqtt-port"), 1883);
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


#include "netMqtt.h"
#include "sysUtils.h"
#include "user_config.h"
#include "board.h"
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
        EthernetClient  tcp;
        MqttClient      client(tcp);
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
    /// 
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

        return;
        Network::Mqtt::client.setId(F("mqtt-tigraha-e7xewj"));
        Network::Mqtt::client.setUsernamePassword(F("admin"), F("123"));


        //последнее слово о разрыве
        String willTopic = F("arduino/will");
        String willPayload = F("oh no!");
        bool willRetain = true;
        int willQos = 1;

        Network::Mqtt::client.beginWill(willTopic, willPayload.length(), willRetain, willQos);
        Network::Mqtt::client.print(willPayload);
        Network::Mqtt::client.endWill();
        //

        String broker = F("dev.rightech.io");
        uint16_t port = 1883;
        if (!Network::Mqtt::client.connect(broker.c_str(), port)) 
        {
            Serial.print("MQTT connection failed! Error code = ");
            Serial.println(Network::Mqtt::client.connectError());
            return;
        }

        // subscribe to a topic
        // the second parameter set's the QoS of the subscription,
        // the the library supports subscribing at QoS 0, 1, or 2
        int subscribeQos = 1;
        String inTopic = F("base/relay/led1");
        Network::Mqtt::client.subscribe(inTopic, subscribeQos);
    }
    ///--------------------------------------------------------------------------------------







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


#include "mqtt_controller.h"
#include <QtMqtt/QMqttTopicFilter>

/*
 * MqttController
 *
 * This class wraps a QMqttClient and exposes simple properties
 * and methods to QML for controlling the mist system.
 *
 * Responsibilities:
 *  - Track MQTT connection state
 *  - Subscribe to mist control topic
 *  - Receive mist state updates
 *  - Publish mist on/off commands
 */

MqttController::MqttController (QMqttClient *client, QObject *parent)
    : QObject(parent), m_client(client)
{
    
    /*
     * Monitor MQTT connection state changes.
     * Updates the 'connected' property when the client connects/disconnects.
     */

    connect (m_client, &QMqttClient :: stateChanged, this, [this] (QMqttClient :: ClientState state) {
        bool nowConnected = (state == QMqttClient :: Connected);
        if (m_connected != nowConnected) {
            m_connected = nowConnected;
            emit connectedChanged();
        }
     });
    
    /*
     * Subscribe to the mist control topic once connected.
     */
    connect(m_client, &QMqttClient::stateChanged, this, [this](QMqttClient::ClientState state){

        if (state == QMqttClient::Connected){
            m_client->subscribe(QMqttTopicFilter("terrarium/mist"), 0);

        }
    });

    /*
     * Handle incoming MQTT messages.
     */
    connect(m_client, &QMqttClient::messageReceived, this, &MqttController::handleMqttmsg);

}
   
/*
 * Handles received MQTT messages.
 * Listens for mist on/off updates.
 */
    void MqttController ::handleMqttmsg(const QByteArray &payload, const QMqttTopicName &topic)
    {

        if (topic.name() == "terrarium/mist"){
            bool newState = (payload == "ON");

            if (m_mistOn != newState){
                m_mistOn = newState;
                emit mistOnChanged();
            }
        }
    }
    
/*
 * Publishes a mist ON/OFF command to MQTT.
 * Called from QML.
 */
    void MqttController::setMist(bool on){
        if (m_client && m_connected) {

            if (m_mistOn != on){
                m_mistOn = on;
                emit mistOnChanged();
            }
            // Publish new state to the broker
            m_client->publish(QMqttTopicName("terrarium/mist"),
                              on ? QByteArray("ON") : QByteArray("OFF"));
        }

    }

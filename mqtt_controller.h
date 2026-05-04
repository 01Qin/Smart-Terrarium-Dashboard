#ifndef MQTT_CONTROLLER_H
#define MQTT_CONTROLLER_H

#pragma once
#include <QObject>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttTopicName>


/*
 * MqttController
 *
 * This class provides a simple interface between the application
 * and an MQTT broker. It is designed to be exposed to QML.
 *
 * Responsibilities:
 *  - Track MQTT connection status
 *  - Subscribe to mist control topic
 *  - Receive mist ON/OFF updates
 *  - Publish mist control commands from the UI
 */
class MqttController : public QObject
{
    Q_OBJECT

    /*
     * Indicates whether the mist system is currently ON.
     * Read-only from QML; updated via MQTT messages or setMist().
     */
    Q_PROPERTY(bool mistOn READ mistOn  NOTIFY mistOnChanged )
    // Indicates whether the MQTT client is connected to the broker.
    Q_PROPERTY(bool connected READ connected  NOTIFY connectedChanged FINAL)

public:

    /*
     * Constructor
     * @param client Pointer to an already configured QMqttClient
     * @param parent Optional QObject parent
     */

    explicit MqttController (QMqttClient *client, QObject *parent = nullptr);
    // Returns the current mist state.
    bool mistOn() const { return m_mistOn; }
    // Returns the current MQTT connection status.
    bool connected() const { return m_connected; }

signals:
    void mistOnChanged();
    void connectedChanged();

public slots:

    /*
     * Handles incoming MQTT messages.
     * Processes mist ON/OFF updates.
     */
    void handleMqttmsg(const QByteArray &payload, const QMqttTopicName &topic);
    Q_INVOKABLE void setMist (bool on);

private:
    // Pointer to the underlying MQTT client.
    QMqttClient *m_client = nullptr;
    bool m_mistOn = false;
    bool m_connected = false;
};

#endif // MQTT_CONTROLLER_H

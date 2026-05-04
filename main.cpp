#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtMqtt/QtMqtt>
#include "EnvironmentModel.h"
#include "OpenMeteo.h"
#include <QQmlContext>
#include "mqtt_controller.h"


/*
 * Entry point of the Smart Terrarium application.
 *
 * Responsibilities:
 *  - Configure software rendering (for compatibility / VM environments)
 *  - Initialize core backend objects (EnvironmentModel, OpenMeteo, MQTT)
 *  - Expose backend objects to QML
 *  - Load the main QML UI
 *  - Start weather data fetching
 */

int main(int argc, char *argv[])
{
    /* ===== Rendering & Input Configuration ===== */
    qputenv("LIBGL_ALWAYS_SOFTWARE", "1");
    qputenv("MESA_LOADER_DRIVER_OVERRIDE", "llvmpipe");
    qputenv("GALLIUM_DRIVER", "llvmpipe");
    qputenv("QT_QUICK_BACKEND", "software");
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication app(argc, argv);
    
    /* ===== Core Application Models ===== */

    // Holds current environment data and history
    EnvironmentModel environment;
    OpenMeteo meteo (&environment);

    
    /* ===== MQTT Setup ===== */

    // MQTT client configuration
    QMqttClient mqttClient;
    mqttClient.setHostname("broker.hivemq.com");
    mqttClient.setPort(1883);
    // Connect to MQTT broke
    mqttClient.connectToHost();
    // Controller that wraps MQTT logic for QML usage
    MqttController mqttController(&mqttClient);

    /* ===== QML Engine Setup ===== */
    QQmlApplicationEngine engine;

    
    /*
     * Expose backend objects to QML.
     * These can be accessed directly in QML as:
     *  - environment
     *  - simulator
     *  - mqtt
     */
    engine.rootContext()->setContextProperty("environment", &environment);
    engine.rootContext()->setContextProperty("simulator", &meteo);
    engine.rootContext()->setContextProperty("mqtt", &mqttController);

    // Exit application if QML root object fails to lo
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    // Load the main QML component
    engine.loadFromModule("Smart_Terrarium", "Main");
    /* ===== Start Background Services ===== */
    meteo.start();

    return QCoreApplication::exec();
}

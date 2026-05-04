#ifndef OPENMETEO_H
#define OPENMETEO_H

#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>

class EnvironmentModel;


/*
 * OpenMeteo
 *
 * This class is responsible for retrieving weather data
 * (temperature and humidity) from the Open-Meteo API.
 *
 * It periodically fetches:
 *  - Current hourly weather data
 *  - Historical daily weather data
 *
 * Retrieved data is written into the EnvironmentModel,
 * 
which is then consumed by the UI (QML).
 */

class OpenMeteo : public QObject {
    Q_OBJECT

public:

    /*
     * Constructor
     * @param environment Pointer to the EnvironmentModel
     *                    that receives fetched data
     * @param parent Optional QObject parent
     */

    explicit OpenMeteo (EnvironmentModel *environment, QObject *parent = nullptr);
    void start(); // start periodic update
    void stop();

private:
    // Manages HTTP requests to the Open-Meteo API.
    QNetworkAccessManager m_network;
    // Timer used to trigger periodic data fetching.
    QTimer m_Timer;
    EnvironmentModel *m_environment;
    // Fetches current temperature and humidity values.
    void fetchData();

    /*
     * Fetches historical weather data
     * (daily max temperature and mean humidity).
     */
    void fetchHistoryData();
};

#endif // OPENMETEO_H

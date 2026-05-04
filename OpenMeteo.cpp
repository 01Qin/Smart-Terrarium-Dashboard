#include "OpenMeteo.h"
#include "EnvironmentModel.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>

/*
 * OpenMeteo
 *
 * This class fetches real-time and historical weather data
 * (temperature and humidity) from the Open-Meteo API and
 * updates the EnvironmentModel accordingly.
 */

/*
 * Constructor
 * @param environment Pointer to the EnvironmentModel to update
 * @param parent Optional QObject parent
 */

OpenMeteo::OpenMeteo(EnvironmentModel *environment, QObject *parent)
    : QObject(parent), m_environment(environment){

    /*
     * Timer triggers periodic data updates.
     * Interval is set to 60,000 ms (intended hourly refresh,
     * but currently equals 1 minute).
     */
    m_Timer.setInterval(60000); 
    connect (&m_Timer, &QTimer::timeout, this, [this](){
        fetchData();
        fetchHistoryData();
    });
}


/*
 * Starts periodic data fetching.
 * Also performs an immediate fetch on start.
 */

void OpenMeteo::start(){
    fetchHistoryData();
    fetchData();
    m_Timer.start();

}
void OpenMeteo::stop(){

    m_Timer.stop();

}


/*
 * Fetches current temperature and humidity values
 * from the Open-Meteo hourly forecast API.
 */

void OpenMeteo::fetchData()
{
    QUrl url(
        "https://api.open-meteo.com/v1/forecast"
        "?latitude=60.2276&longitude=24.8873"
        "&hourly=temperature_2m,relative_humidity_2m"
        "&timezone=auto"
        );

    QNetworkRequest request(url);
    auto reply = m_network.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        // Abort if a network error occurre
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            return;
        }
        
        // Parse JSON response
        const QJsonDocument doc =
            QJsonDocument::fromJson(reply->readAll());
        const QJsonObject root = doc.object();
        const QJsonObject hourly = root["hourly"].toObject();
        
        // Read time array
        const QJsonArray timerArr = hourly["time"].toArray();
        
        // Current hour in UTC (formatted to match API timestamps)
        const QString nowStr = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:00");
        
        // Find index corresponding to current hour
        int idx = 0;
        for (int i =0; i < timerArr.size(); ++i){
            if (timerArr[i].toString() == nowStr){
                idx = i;
                break;
            }
        }

        // Extract temperature and humidity arrays
        const QJsonArray tempArr =
            hourly["temperature_2m"].toArray();
        const QJsonArray humidityArr =
            hourly["relative_humidity_2m"].toArray();

        if (tempArr.isEmpty() || humidityArr.isEmpty()) {
            reply->deleteLater();
            return;
        }
        
        // Read current values
        double temp = tempArr[idx].toDouble();
        double humidity = humidityArr[idx].toDouble();

        m_environment->setTemp(temp);
        m_environment->setHumidity(humidity);
        m_environment->setSource("Open-Meteo");

        reply->deleteLater();
    });
}

/*
 * Fetches historical daily weather data (past 10 days)
 * including max temperature and mean humidity.
 */

void OpenMeteo::fetchHistoryData(){
    QUrl url("https://api.open-meteo.com/v1/forecast"
             "?latitude=60.2276&longitude=24.8873"
             "&past_days=10"
             "&forecast_days=1"
             "&daily=temperature_2m_max,relative_humidity_2m_mean"
             "&timezone=auto"

             );

    QNetworkRequest request(url);
    auto reply = m_network.get(request);

    connect(reply, &QNetworkReply::finished, this, [this,reply] {
        // Abort if request failed
        if (reply->error() != QNetworkReply::NoError){
            reply->deleteLater();
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

        const QJsonObject root = doc.object();
        const QJsonObject daily = root["daily"].toObject();
        QJsonArray tempMax = daily["temperature_2m_max"].toArray();
        QJsonArray humidityMean = daily["relative_humidity_2m_mean"].toArray();
        QJsonArray timeArr = daily["time"].toArray();
        
        // Clear previous history before appending new data
        m_environment->clearHistory();
        
        // Append new history values
        for (int i = 0; i < tempMax.size() && i < humidityMean.size(); ++i){
            m_environment->appendTempHistory(tempMax[i].toDouble());
            m_environment->appendHumidityHistory(humidityMean[i].toDouble());
            
            // Append corresponding date if availa
            if (i < timeArr.size()){
                QDate date = QDate::fromString(timeArr[i].toString(), "yyyy-MM-dd");
                m_environment->appendDateHistory(date.toString("yyyy-MM-dd"));
            }

        }
        reply->deleteLater();


    });
}

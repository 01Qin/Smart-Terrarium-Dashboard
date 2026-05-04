#include "EnvironmentModel.h"


/*
 * Constructor
 * Initializes the EnvironmentModel object with an optional parent QObject.
 */

EnvironmentModel::EnvironmentModel(QObject *parent)
    : QObject (parent) {}

/* ===== Getter methods ===== */
double EnvironmentModel::humidity () const {return m_humidity;}
// Returns the latest temperature value.
double EnvironmentModel::temp () const {return m_temp;}
QString EnvironmentModel::source() const {return m_source;}
QVariantList EnvironmentModel::tempHistory() const {return m_tempHistory;}
QVariantList EnvironmentModel::humidityHistory() const {return m_humidityHistory;}
QVariantList EnvironmentModel::dateHistory() const {return m_dateHistory; }

/* ===== Setter methods ===== */


/*
 * Updates the humidity value.
 * - Does nothing if the value hasn't changed
 * - Appends the value to history
 * - Emits signals so UI components can update
 */
void EnvironmentModel::setHumidity (double value){
    if (m_humidity == value) return;
    m_humidity = value;
    m_valid = true;
    m_humidityHistory.append(value);
    emit humidityChanged(m_humidity);
    emit humidityHistoryChanged();
    if (!qIsNaN(m_temp)){
        m_valid = true;
        emit validChanged();
    }

}


/*
 * Updates the temperature value.
 * - Does nothing if the value hasn't changed
 * - Appends the value to history
 * - Emits signals so UI components can update
 */
void EnvironmentModel::setTemp(double value){
    if(m_temp == value) return;
    m_temp = value;
    m_valid = true;
    m_tempHistory.append(value);
    emit tempChanged(m_temp);
    emit tempHistoryChanged();
    if (!qIsNaN(m_humidity)){
        m_valid = true;
        emit validChanged();
    }

}

// Sets the data source string.
void EnvironmentModel::setSource (const QString &value){
    if (m_source == value) return;
    m_source = value;
    emit sourceChanged(m_source);

}

// Returns whether the model currently holds valid data.
bool EnvironmentModel::valid() const{
    return m_valid;
}

/* ===== History helper methods ===== */

// Appends a temperature value to the history manually.
void EnvironmentModel ::appendTempHistory (double value) {
    m_tempHistory.append(value);
    emit tempHistoryChanged();
}

void EnvironmentModel :: appendHumidityHistory(double value) {
    m_humidityHistory.append(value);
    emit humidityHistoryChanged();
}

void EnvironmentModel::appendDateHistory (const QString &date){
    m_dateHistory.append(date);
    emit dateHistoryChanged();
}

// Clears all stored history data (temperature, humidity, and dates).
void EnvironmentModel::clearHistory(){
    m_tempHistory.clear();
    m_humidityHistory.clear();
    m_dateHistory.clear();
    emit tempHistoryChanged();
    emit humidityHistoryChanged();
    emit dateHistoryChanged();
}


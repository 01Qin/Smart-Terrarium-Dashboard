#ifndef ENVIRONMENTMODEL_H
#define ENVIRONMENTMODEL_H

#pragma once
#include <QObject>
#include <QVariantList>


/*
 * EnvironmentModel
 *
 * This class represents environmental data such as temperature and humidity.
 * It is designed to be exposed to QML using Q_PROPERTY, allowing UI components
 * to react automatically when data changes.
 *
 * The model also stores historical values for temperature, humidity, and dates.
 */

class EnvironmentModel : public QObject {
    Q_OBJECT

    /*
     * Current humidity value.
     * Emits humidityChanged when updated.
     */
    Q_PROPERTY(double humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
    Q_PROPERTY(double temp READ temp WRITE setTemp NOTIFY tempChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QVariantList tempHistory READ tempHistory NOTIFY tempHistoryChanged)
    Q_PROPERTY(QVariantList humidityHistory READ humidityHistory NOTIFY humidityHistoryChanged)
    Q_PROPERTY(QVariantList dateHistory READ dateHistory NOTIFY dateHistoryChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:

    /*
     * Constructor
     * @param parent Optional QObject parent.
     */
    explicit EnvironmentModel (QObject *parent = nullptr);
    bool valid() const;

    /* ===== Getters ===== */
    double humidity() const;
    double temp() const;
    QString source() const;
    QVariantList tempHistory() const;
    QVariantList humidityHistory() const;
    QVariantList dateHistory() const;

public slots:

    // Updates humidity value and appends it to history.
    void setHumidity(double value);
    void setTemp (double value);
    void setSource (const QString &value);

    // Manually appends a temperature value to history.
    void appendTempHistory(double value);
    void appendHumidityHistory(double value);
    void appendDateHistory(const QString &date);
    void clearHistory();

signals:

    // Emitted when humidity value changes.
    void humidityChanged(double value);
    void tempChanged(double value);
    void sourceChanged(const QString &value);

    // Emitted when temperature history is updated.
    void tempHistoryChanged();
    void humidityHistoryChanged();
    void dateHistoryChanged();
    void validChanged();


private:

    // Current humidity value (NaN if not set).
    double m_humidity = qQNaN();
    double m_temp = qQNaN();
    QString m_source = "Unknown";
    QVariantList m_tempHistory;
    QVariantList m_humidityHistory;
    QVariantList m_dateHistory;
    // Indicates whether both temperature and humidity are available.
    bool m_valid = false;
};

#endif // ENVIRONMENTMODEL_H

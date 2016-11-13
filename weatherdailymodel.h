#ifndef WEATHERDAILYMODEL_H
#define WEATHERDAILYMODEL_H

#include <QObject>
#include <QColor>
#include <QTimer>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>
#include "weatherdata.h"

class WeatherDailyModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY cityNameChanged)
    Q_PROPERTY(QString countryID READ countryID WRITE setCountryID NOTIFY countryIDChanged)
    Q_PROPERTY(int daysNumber READ daysNumber NOTIFY daysNumberChanged)

public:

    enum WeatherRoles {
        TimestampRole = Qt::UserRole + 1,
        TemperatureDayRole,
        TemperatureMinRole,
        TemperatureMaxRole,
        TemperatureNightRole,
        TemperatureEveRole,
        TemperatureMornRole,
        PressureRole,
        HumidityRole,
        WeatherConditionIdRole,
        WeatherConditionNameRole,
        WeatherConditionDescriptionRole,
        WeatherConditionIconIdRole,
        CloudsCoverageRole,
        WindSpeedRole,
        WindDegreesRole,
        WindGustRole,
        RainRole,
        SnowRole
    };

    explicit WeatherDailyModel(QAbstractListModel *parent = nullptr);
    virtual ~WeatherDailyModel() {}

    void setCoordinates(const QGeoCoordinate &coordinate);

    QString cityName() const;
    QString countryID() const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE void setUpdateInterval(int updateInterval);
    int daysNumber() const;
    int getTimezoneOffset() const;

signals:
    void cityNameChanged(QString cityName);
    void countryIDChanged(QString countryID);
    void daysNumberChanged(int daysNumber);

private slots:
    void onWeatherDailyRequestFinished();
    void setDaysNumber(int daysNumber);

public slots:
    void requestWeatherUpdate();
    void setCityName(QString cityName);
    void setCountryID(QString countryID);
    void setTimezoneOffset(int TimezoneOffset);

private:
    QNetworkAccessManager _nam;
    QGeoCoordinate m_coordinate;
    qint64 _lastTs;
    QString m_cityName;
    QString m_countryID;
    QVector<WeatherData *> m_dailyList;
    QTimer _updateTimer;
    int _updateInterval;
    int m_daysNumber;
    int m_TimezoneOffset;
};

#endif // WEATHERDAILYMODEL_H

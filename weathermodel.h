#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QObject>
#include <QTimer>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include "weatherforecastdata.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortFilterProxyModel(QObject *parent = 0);

    Q_INVOKABLE void setWeatherDateIndex(int weatherIndex);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    int _weatherDateIndex;
};

class WeatherModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY cityNameChanged)
    Q_PROPERTY(QString countryID READ countryID WRITE setCountryID NOTIFY countryIDChanged)
    Q_PROPERTY(int daysNumber READ daysNumber NOTIFY daysNumberChanged)

public:
    enum WeatherRoles {
        TimestampRole = Qt::UserRole + 1,
        TemperatureRole,
        TemperatureMinRole,
        TemperatureMaxRole,
        PressureRole,
        PressureSeaLevelRole,
        PressureGroundLevelRole,
        HumidityRole,
        WeatherConditionIdRole,
        WeatherConditionNameRole,
        WeatherConditionDescriptionRole,
        WeatherConditionIconIdRole,
        CloudsCoverageRole,
        WindSpeedRole,
        WindDegreesRole,
        WindGustRole,
        Rain3HRole,
        Snow3HRole,
        TimestampStringRole

    };
    WeatherModel(QAbstractItemModel *parent = 0);
    virtual ~WeatherModel() {}

    void setCoordinates(qreal latitude, qreal longitude);

    QString cityName() const;
    QString countryID() const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setUpdateInterval(int updateInterval);
    int daysNumber() const;

    Q_INVOKABLE static qreal kelvin2celsius(qreal kelvin) { return (kelvin - 273.15); }
    Q_INVOKABLE static qreal kelvin2fahrenheit(qreal kelvin) { return (kelvin * (9.0/5.0) - 459.67); }

public slots:
    void requestWeatherUpdate();
    WeatherForecastData *getAverageWeather(int day);
    WeatherForecastData *getWeather(int day, int index);

    void setCityName(QString cityName);
    void setCountryID(QString countryID);
    void setDays(QList<WeatherForecastData*> days);
    int measuresForDate(const QDateTime &date) const;
    int measuresForDate(int index) const;

signals:
    void cityNameChanged(QString cityName);
    void countryIDChanged(QString countryID);
    void daysNumberChanged(int daysNumber);
    void daysChanged(QList<WeatherForecastData*> days);

private slots:
    void onWeatherRequestFinished();
    void setDaysNumber(int daysNumber);
private:
    QNetworkAccessManager _nam;
    qreal _latitude;
    qreal _longitude;

    qint64 _lastTs;
    QString m_cityName;
    QString m_countryID;
    QVector<WeatherForecastData *> m_forecastList;
    QMap<QDate, WeatherForecastData *> _forecastList;
    QTimer _updateTimer;
    int _updateInterval;
    int m_daysNumber;
    QLocale _locale;
    QList<WeatherForecastData*> m_days;
    friend class SortFilterProxyModel;
};




#endif // WEATHERMODEL_H

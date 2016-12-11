#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QObject>
#include <QTimer>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>
#include "weatherdata.h"
#include "weathercommon.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit SortFilterProxyModel(QObject *parent = 0);

    Q_INVOKABLE void setWeatherDateIndex(int weatherIndex);

    int count() const;

public slots:
    void setCount(int count);

signals:
    void countChanged(int count);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    int _weatherDateIndex;
    int m_count;
};

//TODO: subclass from WeatherDailyModel
class WeatherModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY cityNameChanged)
    Q_PROPERTY(QString countryID READ countryID WRITE setCountryID NOTIFY countryIDChanged)
    Q_PROPERTY(int daysNumber READ daysNumber NOTIFY daysNumberChanged)
    Q_PROPERTY(WeatherData* currentWeather READ currentWeather WRITE setCurrentWeather NOTIFY currentWeatherChanged)

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
        TimestampStringRole,
        SunriseRole,
        SunsetRole
    };

    explicit WeatherModel(WeatherCommon *wcommon, QAbstractListModel *parent = 0);
    virtual ~WeatherModel();

    QString cityName() const;
    QString countryID() const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void setUpdateInterval(int updateInterval);

    int daysNumber() const;

    WeatherData *currentWeather() const;

public slots:
    void requestWeatherUpdate();

    void setCityName(QString cityName);
    void setCountryID(QString countryID);
    int measuresForDate(const QDateTime &date) const;
    int measuresForDate(int index) const;
    void setCurrentWeather(WeatherData* currentWeather);

signals:
    void cityNameChanged(QString cityName);
    void countryIDChanged(QString countryID);
    void daysNumberChanged(int daysNumber);
    void currentWeatherChanged(WeatherData* currentWeather);

private slots:
    void onWeatherForecastRequestFinished();
    void onWeatherCurrentRequestFinished();
    void setDaysNumber(int daysNumber);
private:
    WeatherCommon * m_wcommon;
    QNetworkAccessManager _nam;
    qint64 _lastTs;
    QString m_cityName;
    QString m_countryID;
    QVector<WeatherData *> m_forecastList;
    QMap<QDate, WeatherData *> m_forecastDates;
    QTimer _updateTimer;
    int _updateInterval;
    int m_daysNumber;
    QLocale _locale;
    friend class SortFilterProxyModel;
    WeatherData* m_currentWeather;

    QNetworkReply *replyForecast;
    QNetworkReply *replyCurrent;
};




#endif // WEATHERMODEL_H

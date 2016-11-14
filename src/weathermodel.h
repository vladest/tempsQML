#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QObject>
#include <QColor>
#include <QTimer>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>
#include "weatherdata.h"

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

//TODO: subclass from WeatherDailyModel
class WeatherModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString cityName READ cityName WRITE setCityName NOTIFY cityNameChanged)
    Q_PROPERTY(QString countryID READ countryID WRITE setCountryID NOTIFY countryIDChanged)
    Q_PROPERTY(int daysNumber READ daysNumber NOTIFY daysNumberChanged)
    Q_PROPERTY(WeatherData* currentWeather READ currentWeather WRITE setCurrentWeather NOTIFY currentWeatherChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY backgroundColorChanged)

public:

    Q_INVOKABLE static qreal kelvin2celsius(qreal kelvin) { return (kelvin - 273.15); }
    Q_INVOKABLE static qreal kelvin2fahrenheit(qreal kelvin) { return (kelvin * (9.0/5.0) - 459.67); }
    Q_INVOKABLE static int roundup(qreal value) { return qRound(value); }

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
    virtual ~WeatherModel();

    void setCoordinates(const QGeoCoordinate &coordinate);

    QString cityName() const;
    QString countryID() const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE void setUpdateInterval(int updateInterval);
    int daysNumber() const;

    WeatherData *currentWeather() const;
    QColor backgroundColor() const;

    int getTimezoneOffset() const;

    static QList<QColor> colorsTable();

public slots:
    void requestWeatherUpdate();
    WeatherData *getAverageWeather(int day);
    WeatherData *getWeather(int day, int index);

    void setCityName(QString cityName);
    void setCountryID(QString countryID);
    void setDays(QList<WeatherData*> days);
    int measuresForDate(const QDateTime &date) const;
    int measuresForDate(int index) const;
    void setCurrentWeather(WeatherData* currentWeather);
    void setBackgroundColor(qreal temp);
    void setTimezoneOffset(int TimezoneOffset);

signals:
    void cityNameChanged(QString cityName);
    void countryIDChanged(QString countryID);
    void daysNumberChanged(int daysNumber);
    void daysChanged(QList<WeatherData*> days);
    void currentWeatherChanged(WeatherData* currentWeather);
    void backgroundColorChanged(QColor backgroundColor);

private slots:
    void onWeatherForecastRequestFinished();
    void onWeatherCurrentRequestFinished();
    void setDaysNumber(int daysNumber);
private:
    QNetworkAccessManager _nam;
    QGeoCoordinate m_coordinate;
    qint64 _lastTs;
    QString m_cityName;
    QString m_countryID;
    QVector<WeatherData *> m_forecastList;
    QMap<QDate, WeatherData *> _forecastList;
    QTimer _updateTimer;
    int _updateInterval;
    int m_daysNumber;
    QLocale _locale;
    QList<WeatherData*> m_days;
    friend class SortFilterProxyModel;
    WeatherData* m_currentWeather;
    QColor m_backgroundColor;
    int m_TimezoneOffset;

    static QList<QColor> _colorsTable;
};




#endif // WEATHERMODEL_H

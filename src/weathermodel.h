#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QObject>
#include <QColor>
#include <QTimer>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>
#include <QSettings>
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
    Q_PROPERTY(QString temperatureScale READ temperatureScale NOTIFY temperatureScaleChanged)
    Q_PROPERTY(bool showAnimation READ showAnimation WRITE setShowAnimation NOTIFY showAnimationChanged)
    Q_PROPERTY(bool menuBarWeather READ menuBarWeather WRITE setMenuBarWeather NOTIFY menuBarWeatherChanged)
    Q_PROPERTY(bool runAtStartup READ runAtStartup WRITE setRunAtStartup NOTIFY runAtStartupChanged)
    Q_PROPERTY(TemperatureScales tempScale READ tempScale  WRITE setTempScale  NOTIFY tempScaleChanged)

public:

    Q_INVOKABLE static qreal kelvin2celsius(qreal kelvin) { return (kelvin - 273.15); }
    Q_INVOKABLE static qreal kelvin2fahrenheit(qreal kelvin) { return (kelvin * (9.0/5.0) - 459.67); }
    Q_INVOKABLE static int roundup(qreal value) { return qRound(value); }

    enum TemperatureScales {
        Celsium,
        Fahrenheit
    };

    Q_ENUM(TemperatureScales)

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
    Q_INVOKABLE qreal convertToCurrentScale(qreal temp_k);

    int daysNumber() const;

    WeatherData *currentWeather() const;
    QColor backgroundColor() const;

    int getTimezoneOffset() const;

    static QList<QColor> colorsTable();

    QString temperatureScale() const;
    bool showAnimation() const;
    bool menuBarWeather() const;
    bool runAtStartup() const;

    void setTempScale(TemperatureScales tempScale);
    TemperatureScales tempScale() const;

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
    void setShowAnimation(bool showAnimation);
    void setMenuBarWeather(bool menuBarWeather);
    void setRunAtStartup(bool runAtStartup);

signals:
    void cityNameChanged(QString cityName);
    void countryIDChanged(QString countryID);
    void daysNumberChanged(int daysNumber);
    void daysChanged(QList<WeatherData*> days);
    void currentWeatherChanged(WeatherData* currentWeather);
    void backgroundColorChanged(QColor backgroundColor);
    void temperatureScaleChanged(QString temperatureScale);
    void showAnimationChanged(bool showAnimation);
    void menuBarWeatherChanged(bool menuBarWeather);
    void runAtStartupChanged(bool runAtStartup);
    void tempScaleChanged(TemperatureScales tempScale);

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
    bool m_showAnimation;
    bool m_menuBarWeather;
    bool m_runAtStartup;
    TemperatureScales m_tempScale;
    QSettings settings;
};




#endif // WEATHERMODEL_H

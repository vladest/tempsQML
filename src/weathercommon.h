#ifndef WEATHERCOMMON_H
#define WEATHERCOMMON_H

#include <QObject>
#include <QSettings>
#include <QGeoCoordinate>
#include <QColor>

class WeatherCommon : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QString temperatureScale READ temperatureScale NOTIFY temperatureScaleChanged)
    Q_PROPERTY(bool showAnimation READ showAnimation WRITE setShowAnimation NOTIFY showAnimationChanged)
    Q_PROPERTY(bool menuBarWeather READ menuBarWeather WRITE setMenuBarWeather NOTIFY menuBarWeatherChanged)
    Q_PROPERTY(bool runAtStartup READ runAtStartup WRITE setRunAtStartup NOTIFY runAtStartupChanged)
    Q_PROPERTY(TemperatureScales tempScale READ tempScale  WRITE setTempScale  NOTIFY tempScaleChanged)

public:
    explicit WeatherCommon(QObject *parent = nullptr);
    virtual ~WeatherCommon() {}

    enum TemperatureScales {
        Celsium,
        Fahrenheit
    };
    Q_ENUM(TemperatureScales)

    enum SearchCriteria {
        Coordinates,
        CityName
    };
    Q_ENUM(SearchCriteria)

    enum WeatherType {
        Current,
        Daily,
        Forecast
    };
    Q_ENUM(WeatherType)

    Q_INVOKABLE static qreal kelvin2celsius(qreal kelvin) { return (kelvin - 273.15); }
    Q_INVOKABLE static qreal kelvin2fahrenheit(qreal kelvin) { return (kelvin * (9.0/5.0) - 459.67); }
    Q_INVOKABLE static int roundup(qreal value) { return qRound(value); }

    Q_INVOKABLE qreal convertToCurrentScale(qreal temp_k);

    QColor backgroundColor() const;
    int getTimezoneOffset() const;
    static QList<QColor> colorsTable();

    QString temperatureScale() const;
    bool showAnimation() const;
    bool menuBarWeather() const;
    bool runAtStartup() const;

    void setTempScale(TemperatureScales tempScale);
    TemperatureScales tempScale() const;

    QGeoCoordinate getCoordinate() const;

    WeatherCommon::SearchCriteria getSearchCriteria() const;

    QString getSearchCity() const;

signals:
    void backgroundColorChanged(QColor backgroundColor);
    void temperatureScaleChanged(QString temperatureScale);
    void showAnimationChanged(bool showAnimation);
    void menuBarWeatherChanged(bool menuBarWeather);
    void runAtStartupChanged(bool runAtStartup);
    void tempScaleChanged(TemperatureScales tempScale);
    void requestWeatherUpdate();
    void weatherDownloadError(WeatherType weatherType, int weatherError);

public slots:
    void setCoordinates(const QGeoCoordinate &coordinate);
    void setBackgroundColor(qreal temp);
    void setTimezoneOffset(int TimezoneOffset);
    void setShowAnimation(bool showAnimation);
    void setMenuBarWeather(bool menuBarWeather);
    void setRunAtStartup(bool runAtStartup);

    void search(const QString &city);
    void search();

private:
    QGeoCoordinate m_coordinate;
    QColor m_backgroundColor;
    int m_TimezoneOffset;
    static QList<QColor> _colorsTable;
    bool m_showAnimation;
    bool m_menuBarWeather;
    bool m_runAtStartup;
    TemperatureScales m_tempScale;
    QSettings settings;
    WeatherCommon::SearchCriteria m_searchCriteria;
    QString m_searchCity;
};

#endif // WEATHERCOMMON_H

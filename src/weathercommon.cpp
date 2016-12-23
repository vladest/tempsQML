#include "weathercommon.h"
#include <QDebug>

QList<QColor> WeatherCommon::_colorsTable = QList<QColor>()
        << QColor("#444444") //error
        << QColor("#b1695a")
        << QColor("#DB9864")
        << QColor("#E3BB88")
        << QColor("#D0C7A8")
        << QColor("#B1C2A3")
        << QColor("#80BBB2")
        << QColor("#6D9E96")
        << QColor("#8F95A5")
        << QColor("#6F7685");

QMap<QList<int>, QString> WeatherCommon::_videoNamesTable = QMap<QList<int>, QString>();
QMap<QList<int>, QString> WeatherCommon::_videoNamesNightTable = QMap<QList<int>, QString>();

WeatherCommon::WeatherCommon(QObject *parent) : QObject(parent)
  , m_backgroundColor(QColor("#CCCCCC"))
  , m_TimezoneOffset(0)
  , m_showAnimation(true)
  , m_menuBarWeather(true)
  , m_runAtStartup(true)
  , m_tempScale(Celsium)
  , m_searchCriteria(WeatherCommon::Coordinates)
{
    setTempScale((TemperatureScales)settings.value("temp_scale", Celsium).toInt());
    setMenuBarWeather(settings.value("sys_tray", true).toBool());
    setRunAtStartup(settings.value("run_startup", true).toBool());
    setShowAnimation(settings.value("animation_fx", true).toBool());
    setShowVideo(settings.value("video_fx", false).toBool());
    //check what was requested last and made request again
    m_searchCriteria = (WeatherCommon::SearchCriteria)(settings.value("last_searchcriteria", WeatherCommon::Coordinates).toInt());
    m_searchCity = settings.value("last_city", QVariant("")).toString();
    //read and parse coordinates
    double _lat = settings.value("last_lat", 0.0f).toFloat();
    double _lng = settings.value("last_long", 0.0f).toFloat();
    m_coordinate = QGeoCoordinate(_lat, _lng);

    if (m_searchCriteria == WeatherCommon::Coordinates && m_coordinate.isValid()) {
        search();
    } else if (m_searchCriteria == WeatherCommon::CityName && !m_searchCity.isEmpty()) {
        search(m_searchCity);
    }
    //fill up video files table
    _videoNamesTable.insert(QList<int>() << 611 << 612 << 615 << 616, "Clouds-Snow-Rain");
    _videoNamesTable.insert(QList<int>() << 600 << 601 << 602 << 620 << 621 << 622, "Clouds-Snow");
    _videoNamesTable.insert(QList<int>() << 503 << 504 << 521 << 522 << 531
                            << 300 << 301 << 302 << 310 << 311 << 312 << 313 << 314 << 321,
                            "HeavyDarkClouds-Rain");
    _videoNamesTable.insert(QList<int>() << 500 << 501 << 502 << 520, "Sun-HeavyDarkClouds-Rain");
    _videoNamesTable.insert(QList<int>() << 202 << 211 << 212 << 221 << 231 << 232, "HeavyDarkClouds-Rain-Storm");
    _videoNamesTable.insert(QList<int>() << 200 << 201 << 210 << 230, "Sun-HeavyDarkClouds-Rain-Storm");
    _videoNamesTable.insert(QList<int>() << 800, "Sun");
    _videoNamesTable.insert(QList<int>() << 801, "Sun-SemiSmallCloudComing");
    _videoNamesTable.insert(QList<int>() << 802, "Sun-SmallClouds-Coming");
    _videoNamesTable.insert(QList<int>() << 803, "Sun-SemiDarkCloud");
    _videoNamesTable.insert(QList<int>() << 804, "Sun-HavyDarkClouds");
    _videoNamesTable.insert(QList<int>() << 781 << 900 << 902 << 960 << 961 << 962, "Tornado-Twister");
    _videoNamesTable.insert(QList<int>() << 904, "HotSun");
    _videoNamesTable.insert(QList<int>() << 905 << 955 << 957 << 958 , "Wind");
    _videoNamesTable.insert(QList<int>() << 701 << 711 << 741, "smallClouds-Coming");
    _videoNamesTable.insert(QList<int>() << 721 , "Sun-smallClouds-GoingAway");

    _videoNamesNightTable.insert(QList<int>() << 500, "ClearSky-Rain");
    _videoNamesNightTable.insert(QList<int>() << 803, "Moon-Full-BigClouds-GoingAway");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Half-Cloudy-GoingAway");
    _videoNamesNightTable.insert(QList<int>() << 804, "Moon-HavyClouds");
    _videoNamesNightTable.insert(QList<int>() << 600 << 601 << 602 << 611 << 612 << 615 << 616 << 620 << 621 << 622,
                                 "Moon-Snow");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Young");
    _videoNamesNightTable.insert(QList<int>() << 300 << 301 << 302 << 310 << 311 << 312 << 313 << 314, "Moon-Clouds-Rain");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Full-BigClouds");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Half-Cloudy");
    _videoNamesNightTable.insert(QList<int>() << 200 << 201 << 202 << 210 << 211 << 212 << 221 << 230 << 231 << 232,
                                 "Moon-HavyRain-Thunder");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Young-Clouds-Coming");
    _videoNamesNightTable.insert(QList<int>() << 321 << 501 << 502 << 503 << 504 << 511 << 520 << 521 << 522 << 531,
                                 "Moon-Clouds-Showers");
    _videoNamesNightTable.insert(QList<int>() << 802, "Moon-Full-Cloudy");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Half-Fog");
    _videoNamesNightTable.insert(QList<int>() << 801, "Moon-SmallClouds-Coming");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Young-Clouds-GoingAway");
    _videoNamesNightTable.insert(QList<int>() << 741 << 701, "Moon-Fog");
    _videoNamesNightTable.insert(QList<int>() << 800, "Moon-Full");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Half");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-SmallClouds-GoingAway");
    _videoNamesNightTable.insert(QList<int>() << 0, "Moon-Young-Fog");

    if (qgetenv("TEMPSQML_VIDEO_BASE").size() > 0)
        m_currentVideoBasePath = QString(qgetenv("TEMPSQML_VIDEO_BASE"));
    else
        m_currentVideoBasePath = QStringLiteral("http://www.vladest.org/weather/videos/");
}

QString WeatherCommon::currentVideo() const
{
    return m_currentVideo;
}

void WeatherCommon::setCurrentVideo(const WeatherData &wdata)
{
    const QTime &_now = QDateTime::currentDateTime().time();
    bool isNight = ((_now >= wdata.sunset().time() && _now <= QTime(23,59,59)) ||
                    (_now >= QTime(0,1,1) && _now < wdata.sunrise().time() ));
    const QMap<QList<int>, QString> &table = isNight ? _videoNamesNightTable : _videoNamesTable;
    for (int i = 0; i < table.keys().size(); i++) {
        const QList<int> ids = table.keys().at(i);
        if (ids.contains(wdata.weather_codition_id())) {
            m_currentVideo = m_currentVideoBasePath;
            if (isNight)
                m_currentVideo += QStringLiteral("night/");
            m_currentVideo += table[ids] + QStringLiteral(".mp4");
            emit currentVideoChanged(m_currentVideo);
            return;
        }
    }
    qWarning() << "Did not found any media for weather id" << wdata.weather_codition_id() << "is night" << isNight;
}

qreal WeatherCommon::convertToCurrentScale(qreal temp_k)
{
    return m_tempScale == Celsium ? kelvin2celsius(temp_k) : kelvin2fahrenheit(temp_k);
}

QColor WeatherCommon::backgroundColor() const
{
    return m_backgroundColor;
}

void WeatherCommon::setBackgroundColor(qreal temp)
{
    int index = 9;
    qreal temp_c = roundup(kelvin2celsius(temp));
    if (temp_c > 30) {
        index = 1;
    } else if (temp_c > 26) {
        index = 2;
    } else if (temp_c > 22) {
        index = 3;
    } else if (temp_c > 18) {
        index = 4;
    } else if (temp_c > 14) {
        index = 5;
    } else if (temp_c > 10) {
        index = 6;
    } else if (temp_c > 6) {
        index = 7;
    } else if (temp_c > 2) {
        index = 8;
    } else if (temp_c < -273.0) { ///error
        index = 0;
    }

    m_backgroundColor = _colorsTable.at(index);
    emit backgroundColorChanged(temp);
}
void WeatherCommon::setTempScale(TemperatureScales tempScale)
{
    m_tempScale = tempScale;
    settings.setValue("temp_scale", tempScale);
    emit tempScaleChanged(tempScale);
    emit temperatureScaleChanged(temperatureScale());
}

WeatherCommon::TemperatureScales WeatherCommon::tempScale() const
{
    return m_tempScale;
}

QList<QColor> WeatherCommon::colorsTable()
{
    return _colorsTable;
}

QString WeatherCommon::temperatureScale() const
{
    return m_tempScale == Celsium ? "C" :"F";
}

bool WeatherCommon::showAnimation() const
{
    return m_showAnimation;
}

bool WeatherCommon::showVideo() const
{
    return m_showVideo;
}

bool WeatherCommon::menuBarWeather() const
{
    return m_menuBarWeather;
}

bool WeatherCommon::runAtStartup() const
{
    return m_runAtStartup;
}

int WeatherCommon::getTimezoneOffset() const
{
    return m_TimezoneOffset;
}

void WeatherCommon::setCoordinates(const QGeoCoordinate &coordinate)
{
    if (coordinate != m_coordinate) {
        m_coordinate = coordinate;
        emit requestWeatherUpdate();
    }
}

void WeatherCommon::setTimezoneOffset(int TimezoneOffset)
{
    m_TimezoneOffset = TimezoneOffset;
    emit requestWeatherUpdate();
}

void WeatherCommon::setShowAnimation(bool showAnimation)
{
    if (m_showAnimation == showAnimation)
        return;

    m_showAnimation = showAnimation;
    emit showAnimationChanged(showAnimation);
    settings.setValue("animation_fx", showAnimation);
}

void WeatherCommon::setShowVideo(bool showVideo)
{
    if (m_showVideo == showVideo)
        return;

    m_showVideo = showVideo;
    emit showVideoChanged(showVideo);
    settings.setValue("video_fx", showVideo);
}

void WeatherCommon::setMenuBarWeather(bool menuBarWeather)
{
    if (m_menuBarWeather == menuBarWeather)
        return;

    m_menuBarWeather = menuBarWeather;
    emit menuBarWeatherChanged(menuBarWeather);
    settings.setValue("sys_tray", menuBarWeather);
}

void WeatherCommon::setRunAtStartup(bool runAtStartup)
{
    if (m_runAtStartup == runAtStartup)
        return;

    m_runAtStartup = runAtStartup;
    emit runAtStartupChanged(runAtStartup);
    settings.setValue("run_startup", runAtStartup);
}

void WeatherCommon::search(const QString &city)
{
    if (!city.isEmpty()) {
        m_searchCity = city;
        m_searchCriteria = CityName;
        emit requestWeatherUpdate();
    }
}

void WeatherCommon::search()
{
    if (m_coordinate.isValid()) {
        m_searchCriteria = Coordinates;
        emit requestWeatherUpdate();
    }
}

void WeatherCommon::saveLastRequestedWeather(const QString &city)
{
    if (!city.isEmpty()) {
        settings.setValue("last_city", city);
    }
    if (m_coordinate.isValid()) {
        settings.setValue("last_lat", m_coordinate.latitude());
        settings.setValue("last_long", m_coordinate.longitude());
    }
    settings.setValue("last_searchcriteria", m_searchCriteria);
}

QString WeatherCommon::getSearchCity() const
{
    return m_searchCity;
}

WeatherCommon::SearchCriteria WeatherCommon::getSearchCriteria() const
{
    return m_searchCriteria;
}

QGeoCoordinate WeatherCommon::getCoordinate() const
{
    return m_coordinate;
}

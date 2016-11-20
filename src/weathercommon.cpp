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
    }

    m_backgroundColor = _colorsTable.at(index);
    emit backgroundColorChanged(temp);
}
void WeatherCommon::setTempScale(TemperatureScales tempScale)
{
    m_tempScale = tempScale;
    settings.setValue("temp_scale", tempScale);
    qDebug() << "temp scale" << tempScale;
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

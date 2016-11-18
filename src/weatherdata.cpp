#include "weatherdata.h"
#include <QTextStream>
#include "weathercommon.h"

WeatherData::WeatherData(QObject *parent): QObject(parent)
{
    //init values
    m_temp = 273.0;
    m_temp_min = 273.0;
    m_temp_max = 273.0;
    m_temp_night = 273.0;
    m_temp_eve = 273.0;
    m_temp_morn = 273.0;
    m_pressure = 0.0;
    m_pressure_sea_level = 0.0;
    m_pressure_grnd_level = 0.0;
    m_humidity = 0.0;
    m_weather_codition_id = 0;
    m_clouds_coverage = 0.0;
    m_wind_speed = 0.0;
    m_wind_degrees = 0.0;
    m_wind_gust = 0.0;
    m_rain_3h = 0.0;
    m_snow_3h = 0.0;
}

const QString WeatherData::dump() {

   return QString("Weather data dump:" \
             " at: %1 string: %2." \
             " temperature: %3." \
             " temperature min: %4." \
             " temperature max: %5." \
             " pressure:  %6." \
             " pressure ground: %7." \
             " pressure sea: %8." \
             " humidity: %9." \
             " id: %10. name: %11" \
             " description: %12 icon id: %13." \
             " clouds coverage: %14." \
             " wind speed: %15 degrees: %16 gust: %17." \
             " rain_3h: %18." \
             " snow_3h: %19")
            .arg(timestamp().toString())
            .arg(timestamp_string())
            .arg(WeatherCommon::kelvin2celsius(temp()))
            .arg(WeatherCommon::kelvin2celsius(temp_min()))
            .arg(WeatherCommon::kelvin2celsius(temp_max()))
            .arg(pressure())
            .arg(pressure_grnd_level())
            .arg(pressure_sea_level())
            .arg(humidity())
            .arg(weather_codition_id())
            .arg(weather_codition_name())
            .arg(weather_codition_description())
            .arg(weather_codition_icon_id())
            .arg(clouds_coverage())
            .arg(wind_speed())
            .arg(wind_degrees())
            .arg(wind_gust())
            .arg(rain_3h())
            .arg(snow_3h());
}

QDateTime WeatherData::timestamp() const
{
    return m_timestamp;
}

qreal WeatherData::temp() const
{
    return m_temp;
}

qreal WeatherData::temp_min() const
{
    return m_temp_min;
}

qreal WeatherData::temp_max() const
{
    return m_temp_max;
}

qreal WeatherData::pressure() const
{
    return m_pressure;
}

qreal WeatherData::pressure_sea_level() const
{
    return m_pressure_sea_level;
}

qreal WeatherData::pressure_grnd_level() const
{
    return m_pressure_grnd_level;
}

qreal WeatherData::humidity() const
{
    return m_humidity;
}

int WeatherData::weather_codition_id() const
{
    return m_weather_codition_id;
}

QString WeatherData::weather_codition_name() const
{
    return m_weather_codition_name;
}

QString WeatherData::weather_codition_description() const
{
    return m_weather_codition_description;
}

QString WeatherData::weather_codition_icon_id() const
{
    return m_weather_codition_icon_id;
}

qreal WeatherData::clouds_coverage() const
{
    return m_clouds_coverage;
}

qreal WeatherData::wind_speed() const
{
    return m_wind_speed;
}

qreal WeatherData::wind_degrees() const
{
    return m_wind_degrees;
}

qreal WeatherData::wind_gust() const
{
    return m_wind_gust;
}

qreal WeatherData::rain_3h() const
{
    return m_rain_3h;
}

qreal WeatherData::snow_3h() const
{
    return m_snow_3h;
}

QString WeatherData::timestamp_string() const
{
    return m_timestamp_string;
}

qreal WeatherData::temp_morn() const
{
    return m_temp_morn;
}

qreal WeatherData::temp_eve() const
{
    return m_temp_eve;
}

qreal WeatherData::temp_night() const
{
    return m_temp_night;
}

void WeatherData::set_timestamp(QDateTime timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
    emit timestamp_changed(timestamp);
}

void WeatherData::set_temp(qreal temp)
{
    if (m_temp == temp)
        return;

    m_temp = temp;
    emit temp_changed(temp);
}

void WeatherData::set_temp_min(qreal temp_min)
{
    if (m_temp_min == temp_min)
        return;

    m_temp_min = temp_min;
    emit temp_min_changed(temp_min);
}

void WeatherData::set_temp_max(qreal temp_max)
{
    if (m_temp_max == temp_max)
        return;

    m_temp_max = temp_max;
    emit temp_max_changed(temp_max);
}

void WeatherData::set_pressure(qreal pressure)
{
    if (m_pressure == pressure)
        return;

    m_pressure = pressure;
    emit pressure_changed(pressure);
}

void WeatherData::set_pressure_sea_level(qreal pressure_sea_level)
{
    if (m_pressure_sea_level == pressure_sea_level)
        return;

    m_pressure_sea_level = pressure_sea_level;
    emit pressure_sea_level_changed(pressure_sea_level);
}

void WeatherData::set_humidity(qreal humidity)
{
    if (m_humidity == humidity)
        return;

    m_humidity = humidity;
    emit humidity_changed(humidity);
}

void WeatherData::set_weather_codition_id(int weather_codition_id)
{
    if (m_weather_codition_id == weather_codition_id)
        return;

    m_weather_codition_id = weather_codition_id;
    emit weather_codition_id_changed(weather_codition_id);
}

void WeatherData::set_weather_codition_name(QString weather_codition_name)
{
    if (m_weather_codition_name == weather_codition_name)
        return;

    m_weather_codition_name = weather_codition_name;
    emit weather_codition_name_changed(weather_codition_name);
}

void WeatherData::set_weather_codition_description(QString weather_codition_description)
{
    if (m_weather_codition_description == weather_codition_description)
        return;

    m_weather_codition_description = weather_codition_description;
    emit weather_codition_description_changed(weather_codition_description);
}

void WeatherData::set_weather_codition_icon_id(QString weather_codition_icon_id)
{
    if (m_weather_codition_icon_id == weather_codition_icon_id)
        return;

    m_weather_codition_icon_id = weather_codition_icon_id;
    emit weather_codition_icon_id_changed(weather_codition_icon_id);
}

void WeatherData::set_clouds_coverage(qreal clouds_coverage)
{
    if (m_clouds_coverage == clouds_coverage)
        return;

    m_clouds_coverage = clouds_coverage;
    emit clouds_coverage_changed(clouds_coverage);
}

void WeatherData::set_wind_speed(qreal wind_speed)
{
    if (m_wind_speed == wind_speed)
        return;

    m_wind_speed = wind_speed;
    emit wind_speed_changed(wind_speed);
}

void WeatherData::set_wind_degrees(qreal wind_degrees)
{
    if (m_wind_degrees == wind_degrees)
        return;

    m_wind_degrees = wind_degrees;
    emit wind_degrees_changed(wind_degrees);
}

void WeatherData::set_wind_gust(qreal wind_gust)
{
    if (m_wind_gust == wind_gust)
        return;

    m_wind_gust = wind_gust;
    emit wind_gust_changed(wind_gust);
}

void WeatherData::set_rain_3h(qreal rain_3h)
{
    if (m_rain_3h == rain_3h)
        return;

    m_rain_3h = rain_3h;
    emit rain_3h_changed(rain_3h);
}

void WeatherData::set_snow_3h(qreal snow_3h)
{
    if (m_snow_3h == snow_3h)
        return;

    m_snow_3h = snow_3h;
    emit snow_3h_changed(snow_3h);
}

void WeatherData::set_timestamp_string(QString timestamp_string)
{
    if (m_timestamp_string == timestamp_string)
        return;

    m_timestamp_string = timestamp_string;
    emit timestamp_string_changed(timestamp_string);
}

void WeatherData::set_pressure_grnd_level(qreal pressure_grnd_level)
{
    if (m_pressure_grnd_level == pressure_grnd_level)
        return;

    m_pressure_grnd_level = pressure_grnd_level;
    emit pressure_grnd_level_changed(pressure_grnd_level);
}

void WeatherData::set_temp_morn(qreal temp_morn)
{
    if (m_temp_morn == temp_morn)
        return;

    m_temp_morn = temp_morn;
    emit temp_morn_changed(temp_morn);
}

void WeatherData::set_temp_eve(qreal temp_eve)
{
    if (m_temp_eve == temp_eve)
        return;

    m_temp_eve = temp_eve;
    emit temp_eve_changed(temp_eve);
}

void WeatherData::set_temp_night(qreal temp_night)
{
    if (m_temp_night == temp_night)
        return;

    m_temp_night = temp_night;
    emit temp_night_changed(temp_night);
}

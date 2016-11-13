#include "weatherforecastdata.h"
#include <QTextStream>
#include "weathermodel.h"

WeatherForecastData::WeatherForecastData(QObject *parent): QObject(parent)
{
    //init values
    m_temp = 0.0;
    m_temp_min = 0.0;
    m_temp_max = 0.0;
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

const QString WeatherForecastData::dump() {

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
            .arg(WeatherModel::kelvin2celsius(temp()))
            .arg(WeatherModel::kelvin2celsius(temp_min()))
            .arg(WeatherModel::kelvin2celsius(temp_max()))
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

QDateTime WeatherForecastData::timestamp() const
{
    return m_timestamp;
}

qreal WeatherForecastData::temp() const
{
    return m_temp;
}

qreal WeatherForecastData::temp_min() const
{
    return m_temp_min;
}

qreal WeatherForecastData::temp_max() const
{
    return m_temp_max;
}

qreal WeatherForecastData::pressure() const
{
    return m_pressure;
}

qreal WeatherForecastData::pressure_sea_level() const
{
    return m_pressure_sea_level;
}

qreal WeatherForecastData::pressure_grnd_level() const
{
    return m_pressure_grnd_level;
}

qreal WeatherForecastData::humidity() const
{
    return m_humidity;
}

int WeatherForecastData::weather_codition_id() const
{
    return m_weather_codition_id;
}

QString WeatherForecastData::weather_codition_name() const
{
    return m_weather_codition_name;
}

QString WeatherForecastData::weather_codition_description() const
{
    return m_weather_codition_description;
}

QString WeatherForecastData::weather_codition_icon_id() const
{
    return m_weather_codition_icon_id;
}

qreal WeatherForecastData::clouds_coverage() const
{
    return m_clouds_coverage;
}

qreal WeatherForecastData::wind_speed() const
{
    return m_wind_speed;
}

qreal WeatherForecastData::wind_degrees() const
{
    return m_wind_degrees;
}

qreal WeatherForecastData::wind_gust() const
{
    return m_wind_gust;
}

qreal WeatherForecastData::rain_3h() const
{
    return m_rain_3h;
}

qreal WeatherForecastData::snow_3h() const
{
    return m_snow_3h;
}

QString WeatherForecastData::timestamp_string() const
{
    return m_timestamp_string;
}

void WeatherForecastData::set_timestamp(QDateTime timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
    emit timestamp_changed(timestamp);
}

void WeatherForecastData::set_temp(qreal temp)
{
    if (m_temp == temp)
        return;

    m_temp = temp;
    emit temp_changed(temp);
}

void WeatherForecastData::set_temp_min(qreal temp_min)
{
    if (m_temp_min == temp_min)
        return;

    m_temp_min = temp_min;
    emit temp_min_changed(temp_min);
}

void WeatherForecastData::set_temp_max(qreal temp_max)
{
    if (m_temp_max == temp_max)
        return;

    m_temp_max = temp_max;
    emit temp_max_changed(temp_max);
}

void WeatherForecastData::set_pressure(qreal pressure)
{
    if (m_pressure == pressure)
        return;

    m_pressure = pressure;
    emit pressure_changed(pressure);
}

void WeatherForecastData::set_pressure_sea_level(qreal pressure_sea_level)
{
    if (m_pressure_sea_level == pressure_sea_level)
        return;

    m_pressure_sea_level = pressure_sea_level;
    emit pressure_sea_level_changed(pressure_sea_level);
}

void WeatherForecastData::set_humidity(qreal humidity)
{
    if (m_humidity == humidity)
        return;

    m_humidity = humidity;
    emit humidity_changed(humidity);
}

void WeatherForecastData::set_weather_codition_id(int weather_codition_id)
{
    if (m_weather_codition_id == weather_codition_id)
        return;

    m_weather_codition_id = weather_codition_id;
    emit weather_codition_id_changed(weather_codition_id);
}

void WeatherForecastData::set_weather_codition_name(QString weather_codition_name)
{
    if (m_weather_codition_name == weather_codition_name)
        return;

    m_weather_codition_name = weather_codition_name;
    emit weather_codition_name_changed(weather_codition_name);
}

void WeatherForecastData::set_weather_codition_description(QString weather_codition_description)
{
    if (m_weather_codition_description == weather_codition_description)
        return;

    m_weather_codition_description = weather_codition_description;
    emit weather_codition_description_changed(weather_codition_description);
}

void WeatherForecastData::set_weather_codition_icon_id(QString weather_codition_icon_id)
{
    if (m_weather_codition_icon_id == weather_codition_icon_id)
        return;

    m_weather_codition_icon_id = weather_codition_icon_id;
    emit weather_codition_icon_id_changed(weather_codition_icon_id);
}

void WeatherForecastData::set_clouds_coverage(qreal clouds_coverage)
{
    if (m_clouds_coverage == clouds_coverage)
        return;

    m_clouds_coverage = clouds_coverage;
    emit clouds_coverage_changed(clouds_coverage);
}

void WeatherForecastData::set_wind_speed(qreal wind_speed)
{
    if (m_wind_speed == wind_speed)
        return;

    m_wind_speed = wind_speed;
    emit wind_speed_changed(wind_speed);
}

void WeatherForecastData::set_wind_degrees(qreal wind_degrees)
{
    if (m_wind_degrees == wind_degrees)
        return;

    m_wind_degrees = wind_degrees;
    emit wind_degrees_changed(wind_degrees);
}

void WeatherForecastData::set_wind_gust(qreal wind_gust)
{
    if (m_wind_gust == wind_gust)
        return;

    m_wind_gust = wind_gust;
    emit wind_gust_changed(wind_gust);
}

void WeatherForecastData::set_rain_3h(qreal rain_3h)
{
    if (m_rain_3h == rain_3h)
        return;

    m_rain_3h = rain_3h;
    emit rain_3h_changed(rain_3h);
}

void WeatherForecastData::set_snow_3h(qreal snow_3h)
{
    if (m_snow_3h == snow_3h)
        return;

    m_snow_3h = snow_3h;
    emit snow_3h_changed(snow_3h);
}

void WeatherForecastData::set_timestamp_string(QString timestamp_string)
{
    if (m_timestamp_string == timestamp_string)
        return;

    m_timestamp_string = timestamp_string;
    emit timestamp_string_changed(timestamp_string);
}

void WeatherForecastData::set_pressure_grnd_level(qreal pressure_grnd_level)
{
    if (m_pressure_grnd_level == pressure_grnd_level)
        return;

    m_pressure_grnd_level = pressure_grnd_level;
    emit pressure_grnd_level_changed(pressure_grnd_level);
}

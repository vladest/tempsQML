#ifndef WEATHERFORECASTDATA_H
#define WEATHERFORECASTDATA_H

#include <QObject>
#include <QDateTime>

class WeatherData: public QObject {
    Q_OBJECT

    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE set_timestamp NOTIFY timestamp_changed) //weather it the time
    Q_PROPERTY(qreal temp READ temp WRITE set_temp NOTIFY temp_changed)
    Q_PROPERTY(qreal temp_min READ temp_min WRITE set_temp_min NOTIFY temp_min_changed)
    Q_PROPERTY(qreal temp_max READ temp_max WRITE set_temp_max NOTIFY temp_max_changed)
    Q_PROPERTY(qreal temp_morn READ temp_morn WRITE set_temp_morn NOTIFY temp_morn_changed)
    Q_PROPERTY(qreal temp_eve READ temp_eve WRITE set_temp_eve NOTIFY temp_eve_changed)
    Q_PROPERTY(qreal temp_night READ temp_night WRITE set_temp_night NOTIFY temp_night_changed)
    Q_PROPERTY(qreal pressure READ pressure WRITE set_pressure NOTIFY pressure_changed)
    Q_PROPERTY(qreal pressure_sea_level READ pressure_sea_level WRITE set_pressure_sea_level NOTIFY pressure_sea_level_changed)
    Q_PROPERTY(qreal pressure_grnd_level READ pressure_grnd_level WRITE set_pressure_grnd_level NOTIFY pressure_grnd_level_changed)
    Q_PROPERTY(qreal humidity READ humidity WRITE set_humidity NOTIFY humidity_changed)
    Q_PROPERTY(int weather_codition_id READ weather_codition_id WRITE set_weather_codition_id NOTIFY weather_codition_id_changed)
    Q_PROPERTY(QString weather_codition_name READ weather_codition_name WRITE set_weather_codition_name NOTIFY weather_codition_name_changed)
    Q_PROPERTY(QString weather_codition_description READ weather_codition_description WRITE set_weather_codition_description NOTIFY weather_codition_description_changed)
    Q_PROPERTY(QString weather_codition_icon_id READ weather_codition_icon_id WRITE set_weather_codition_icon_id NOTIFY weather_codition_icon_id_changed)
    Q_PROPERTY(qreal clouds_coverage READ clouds_coverage WRITE set_clouds_coverage NOTIFY clouds_coverage_changed)
    Q_PROPERTY(qreal wind_speed READ wind_speed WRITE set_wind_speed NOTIFY wind_speed_changed)
    Q_PROPERTY(qreal wind_degrees READ wind_degrees WRITE set_wind_degrees NOTIFY wind_degrees_changed)
    Q_PROPERTY(qreal wind_gust READ wind_gust WRITE set_wind_gust NOTIFY wind_gust_changed)
    Q_PROPERTY(qreal rain_3h READ rain_3h WRITE set_rain_3h NOTIFY rain_3h_changed)
    Q_PROPERTY(qreal snow_3h READ snow_3h WRITE set_snow_3h NOTIFY snow_3h_changed)
    Q_PROPERTY(QString timestamp_string READ timestamp_string WRITE set_timestamp_string NOTIFY timestamp_string_changed)
public:
    WeatherData(QObject *parent = 0);
    virtual ~WeatherData() {}
    const QString dump();

    QDateTime timestamp() const;
    qreal temp() const;
    qreal temp_min() const;
    qreal temp_max() const;
    qreal pressure() const;
    qreal pressure_sea_level() const;
    qreal pressure_grnd_level() const;
    qreal humidity() const;
    int weather_codition_id() const;
    QString weather_codition_name() const;
    QString weather_codition_description() const;
    QString weather_codition_icon_id() const;
    qreal clouds_coverage() const;
    qreal wind_speed() const;
    qreal wind_degrees() const;
    qreal wind_gust() const;
    qreal rain_3h() const;
    qreal snow_3h() const;
    QString timestamp_string() const;
//    bool operator<(const WeatherForecastData &i1) const
//    {
//       return m_timestamp < i1.m_timestamp;
//    }
//    inline bool operator<(const WeatherForecastData &w1, const WeatherForecastData &w2)
//    {
//        return w1.timestamp() < w2.timestamp();
    //    }
    qreal temp_morn() const;
    qreal temp_eve() const;
    qreal temp_night() const;

public slots:
    void set_timestamp(QDateTime timestamp);
    void set_temp(qreal temp);
    void set_temp_min(qreal temp_min);
    void set_temp_max(qreal temp_max);
    void set_pressure(qreal pressure);
    void set_pressure_sea_level(qreal pressure_sea_level);
    void set_humidity(qreal humidity);
    void set_weather_codition_id(int weather_codition_id);
    void set_weather_codition_name(QString weather_codition_name);
    void set_weather_codition_description(QString weather_codition_description);
    void set_weather_codition_icon_id(QString weather_codition_icon_id);
    void set_clouds_coverage(qreal clouds_coverage);
    void set_wind_speed(qreal wind_speed);
    void set_wind_degrees(qreal wind_degrees);
    void set_wind_gust(qreal wind_gust);
    void set_rain_3h(qreal rain_3h);
    void set_snow_3h(qreal snow_3h);
    void set_timestamp_string(QString timestamp_string);
    void set_pressure_grnd_level(qreal pressure_grnd_level);
    void set_temp_morn(qreal temp_morn);
    void set_temp_eve(qreal temp_eve);
    void set_temp_night(qreal temp_night);

signals:
    void timestamp_changed(QDateTime timestamp);
    void temp_changed(qreal temp);
    void temp_min_changed(qreal temp_min);
    void temp_max_changed(qreal temp_max);
    void pressure_changed(qreal pressure);
    void pressure_sea_level_changed(qreal pressure_sea_level);
    void humidity_changed(qreal humidity);
    void weather_codition_id_changed(int weather_codition_id);
    void weather_codition_name_changed(QString weather_codition_name);
    void weather_codition_description_changed(QString weather_codition_description);
    void weather_codition_icon_id_changed(QString weather_codition_icon_id);
    void clouds_coverage_changed(qreal clouds_coverage);
    void wind_speed_changed(qreal wind_speed);
    void wind_degrees_changed(qreal wind_degrees);
    void wind_gust_changed(qreal wind_gust);
    void rain_3h_changed(qreal rain_3h);
    void snow_3h_changed(qreal snow_3h);
    void timestamp_string_changed(QString timestamp_string);
    void pressure_grnd_level_changed(qreal pressure_grnd_level);

    void temp_morn_changed(qreal temp_morn);

    void temp_eve_changed(qreal temp_eve);

    void temp_night_changed(qreal temp_night);

private:

    QDateTime m_timestamp;
    qreal m_temp;
    qreal m_temp_min;
    qreal m_temp_max;
    qreal m_pressure;
    qreal m_pressure_sea_level;
    qreal m_pressure_grnd_level;
    qreal m_humidity;
    int m_weather_codition_id;
    QString m_weather_codition_name;
    QString m_weather_codition_description;
    QString m_weather_codition_icon_id;
    qreal m_clouds_coverage;
    qreal m_wind_speed;
    qreal m_wind_degrees;
    qreal m_wind_gust;
    qreal m_rain_3h;
    qreal m_snow_3h;
    QString m_timestamp_string;
    qreal m_temp_morn;
    qreal m_temp_eve;
    qreal m_temp_night;
};


#endif // WEATHERFORECASTDATA_H

#include "weatherdailymodel.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QQmlEngine>

const QString weatherDailyUrl = "http://api.openweathermap.org/data/2.5/forecast/daily?";
const QString appID = "&appid=483240f16c1aa9e5feab1fa41c9d273a";

WeatherDailyModel::WeatherDailyModel(WeatherCommon *wcommon, QAbstractListModel *parent) : QAbstractListModel(parent)
  , _updateInterval(-1)
  , m_daysNumber(0)
  , m_wcommon(wcommon)
  , replyDaily(nullptr)
{
    connect(&_updateTimer, &QTimer::timeout, this, &WeatherDailyModel::requestWeatherUpdate);
    connect(wcommon, &WeatherCommon::requestWeatherUpdate, this, &WeatherDailyModel::requestWeatherUpdate);
    _updateTimer.setSingleShot(false);
}

void WeatherDailyModel::requestWeatherUpdate()
{
    QUrl dailyurl;

    if (replyDaily)
        replyDaily->abort();
    if (m_wcommon->getSearchCriteria() == WeatherCommon::Coordinates) {
        const QGeoCoordinate &coord = m_wcommon->getCoordinate();
        dailyurl = QUrl(QString("%1lat=%2&lon=%3%4").arg(weatherDailyUrl).arg(coord.latitude()).arg(coord.longitude()).arg(appID));
    } else {
        dailyurl = QUrl(QString("%1q=%2%3").arg(weatherDailyUrl).arg(m_wcommon->getSearchCity()).arg(appID));
    }
    QNetworkRequest req = QNetworkRequest(dailyurl);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    QNetworkReply *replyDaily = _nam.get(QNetworkRequest(req));
    connect(replyDaily, &QNetworkReply::finished, this, &WeatherDailyModel::onWeatherDailyRequestFinished);
}

QString WeatherDailyModel::cityName() const
{
    return m_cityName;
}

QString WeatherDailyModel::countryID() const
{
    return m_countryID;
}

QHash<int, QByteArray> WeatherDailyModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TimestampRole] = "timestamp";
    roles[TemperatureDayRole] = "temp_day";
    roles[TemperatureMinRole] = "temp_min";
    roles[TemperatureMaxRole] = "temp_max";
    roles[TemperatureNightRole] = "temp_night";
    roles[TemperatureEveRole] = "temp_eve";
    roles[TemperatureMornRole] = "temp_morn";
    roles[PressureRole] = "pressure";
    roles[HumidityRole] = "humidity";
    roles[WeatherConditionIdRole] = "weather_codition_id";
    roles[WeatherConditionNameRole] = "weather_codition_name";
    roles[WeatherConditionDescriptionRole] = "weather_codition_description";
    roles[WeatherConditionIconIdRole] = "weather_codition_icon_id";
    roles[CloudsCoverageRole] = "clouds_coverage";
    roles[WindSpeedRole] = "wind_speed";
    roles[WindDegreesRole] = "wind_degrees";
    roles[WindGustRole] = "wind_gust";
    roles[RainRole] = "rain";
    roles[SnowRole] = "snow";
    return roles;
}

int WeatherDailyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_dailyList.size();
}

QVariant WeatherDailyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_dailyList.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == WeatherConditionNameRole)
        return m_dailyList.at(index.row())->weather_codition_name();
    else if (role == TimestampRole)
        return m_dailyList.at(index.row())->timestamp();
    else if (role == TemperatureDayRole)
        return m_dailyList.at(index.row())->temp();
    else if (role == TemperatureMinRole)
        return m_dailyList.at(index.row())->temp_min();
    else if (role == TemperatureMaxRole)
        return m_dailyList.at(index.row())->temp_max();
    else if (role == TemperatureEveRole)
        return m_dailyList.at(index.row())->temp_eve();
    else if (role == TemperatureMornRole)
        return m_dailyList.at(index.row())->temp_morn();
    else if (role == TemperatureNightRole)
        return m_dailyList.at(index.row())->temp_night();
    else if (role == PressureRole)
        return m_dailyList.at(index.row())->pressure();
    else if (role == HumidityRole)
        return m_dailyList.at(index.row())->humidity();
    else if (role == WeatherConditionIdRole)
        return m_dailyList.at(index.row())->weather_codition_id();
    else if (role == WeatherConditionDescriptionRole)
        return m_dailyList.at(index.row())->weather_codition_description();
    else if (role == WeatherConditionIconIdRole)
        return m_dailyList.at(index.row())->weather_codition_icon_id();
    else if (role == CloudsCoverageRole)
        return m_dailyList.at(index.row())->clouds_coverage();
    else if (role == WindSpeedRole)
        return m_dailyList.at(index.row())->wind_speed();
    else if (role == WindDegreesRole)
        return m_dailyList.at(index.row())->wind_degrees();
    else if (role == WindGustRole)
        return m_dailyList.at(index.row())->wind_gust();
    else if (role == RainRole)
        return m_dailyList.at(index.row())->rain_3h();
    else if (role == SnowRole)
        return m_dailyList.at(index.row())->snow_3h();
    else
        return QVariant();
}

void WeatherDailyModel::setCityName(QString cityName)
{
    if (m_cityName == cityName)
        return;

    m_cityName = cityName;
    emit cityNameChanged(cityName);
}

void WeatherDailyModel::setCountryID(QString countryID)
{
    if (m_countryID == countryID)
        return;

    m_countryID = countryID;
    emit countryIDChanged(countryID);
}

void WeatherDailyModel::setDaysNumber(int daysNumber)
{
    if (m_daysNumber == daysNumber)
        return;

    m_daysNumber = daysNumber;
    qDebug() << "days number" << daysNumber;
    emit daysNumberChanged(daysNumber);
}

void WeatherDailyModel::onWeatherDailyRequestFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError){
        QByteArray arr = reply->readAll();
        QJsonDocument  jsonDoc = QJsonDocument::fromJson(arr);

        QJsonObject obj = jsonDoc.object();

        int cod = obj.value("cod").toString().toInt();
        if (cod != 200) {
            qDebug() << "Error requestin daily weather data" << cod << reply->url();
            emit m_wcommon->weatherDownloadError(WeatherCommon::Daily, cod);
            replyDaily = nullptr;
            return;
        }

        QJsonArray weatherList = obj.value("list").toArray();
        beginResetModel();
        qDeleteAll(m_dailyList);
        m_dailyList.clear();

        foreach (const QJsonValue & value, weatherList) {
            QJsonObject obj1 = value.toObject();
            if (!obj1.isEmpty()) {
                WeatherData *wData = new WeatherData;
                QQmlEngine::setObjectOwnership(wData, QQmlEngine::CppOwnership);
                wData->set_timestamp(QDateTime::fromTime_t(obj1.value("dt").toInt(), Qt::UTC, m_wcommon->getTimezoneOffset()));
                QJsonObject tempObj = obj1.value("temp").toObject();

                if (!tempObj.isEmpty()) {
                    wData->set_temp(tempObj.value("day").toDouble());
                    wData->set_temp_min(tempObj.value("min").toDouble());
                    wData->set_temp_max(tempObj.value("max").toDouble());
                    wData->set_temp_eve(tempObj.value("eve").toDouble());
                    wData->set_temp_morn(tempObj.value("morn").toDouble());
                    wData->set_temp_night(tempObj.value("night").toDouble());
                }

                wData->set_pressure(obj1.value("pressure").toDouble());
                wData->set_humidity(obj1.value("humidity").toDouble());
                wData->set_wind_speed(obj1.value("speed").toDouble());
                wData->set_wind_degrees(obj1.value("deg").toDouble());
                wData->set_clouds_coverage(obj1.value("clouds").toDouble());

                QJsonArray weatherarray = obj1.value("weather").toArray();
                foreach (const QJsonValue & wvalue, weatherarray) {
                    const QJsonObject &weatherObj = wvalue.toObject();
                    if (!weatherObj.isEmpty()) {
                        wData->set_weather_codition_name(weatherObj.value("main").toString());
                        wData->set_weather_codition_id(weatherObj.value("id").toInt());
                        wData->set_weather_codition_icon_id(weatherObj.value("icon").toString());
                        wData->set_weather_codition_description(weatherObj.value("description").toString());
                    }
                    break;
                }

                wData->set_rain_3h(obj1.value("rain").toDouble());
                wData->set_snow_3h(obj1.value("snow").toDouble());
                m_dailyList.append(wData);
                //qDebug() << wData->dump() << "\n";
            }
        }
        endResetModel();
    } else {
        qDebug() << "Error requestin daily weather data" << reply->error() << reply->url();
        emit m_wcommon->weatherDownloadError(WeatherCommon::Daily, reply->error());
        m_wcommon->setBackgroundColor(0.0f);
    }
    replyDaily = nullptr;
}

void WeatherDailyModel::setUpdateInterval(int updateInterval)
{
    _updateInterval = updateInterval;
    if (updateInterval > 0) {
        _updateTimer.setInterval(updateInterval*1000);
        _updateTimer.start();
    } else {
        _updateTimer.stop();
    }
}

int WeatherDailyModel::daysNumber() const
{
    return m_daysNumber;
}

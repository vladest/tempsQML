#include "weathermodel.h"
#include <QtQml/QQmlEngine>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

const QString weatherForecastUrl = "http://api.openweathermap.org/data/2.5/forecast?";
const QString weatherCurrentUrl = "http://api.openweathermap.org/data/2.5/weather?";
const QString appID = "&appid=483240f16c1aa9e5feab1fa41c9d273a";

WeatherModel::WeatherModel(WeatherCommon *wcommon, QAbstractListModel *parent):  QAbstractListModel(parent)
  , m_wcommon(wcommon)
  , _updateInterval(-1)
  , m_daysNumber(0)
  , m_currentWeather(new WeatherData)
  , replyForecast(nullptr)
  , replyCurrent(nullptr)
{
    QQmlEngine::setObjectOwnership(m_currentWeather, QQmlEngine::CppOwnership);
    connect(&_updateTimer, &QTimer::timeout, this, &WeatherModel::requestWeatherUpdate);
    connect(wcommon, &WeatherCommon::requestWeatherUpdate, this, &WeatherModel::requestWeatherUpdate);
    _updateTimer.setSingleShot(false);
}

WeatherModel::~WeatherModel() {
    delete m_currentWeather;
}

void WeatherModel::requestWeatherUpdate()
{
    QUrl forecasturl;
    QUrl currentturl;

    if (replyCurrent)
        replyCurrent->abort();
    if (replyForecast)
        replyForecast->abort();
    if (m_wcommon->getSearchCriteria() == WeatherCommon::Coordinates) {
        const QGeoCoordinate &coord = m_wcommon->getCoordinate();
        forecasturl = QUrl(QString("%1lat=%2&lon=%3%4").arg(weatherForecastUrl).arg(coord.latitude()).arg(coord.longitude()).arg(appID));
        currentturl = QUrl(QString("%1lat=%2&lon=%3%4").arg(weatherCurrentUrl).arg(coord.latitude()).arg(coord.longitude()).arg(appID));
    } else {
        forecasturl = QUrl(QString("%1q=%2%3").arg(weatherForecastUrl).arg(m_wcommon->getSearchCity()).arg(appID));
        currentturl = QUrl(QString("%1q=%2%3").arg(weatherCurrentUrl).arg(m_wcommon->getSearchCity()).arg(appID));
    }

    QNetworkRequest req = QNetworkRequest(forecasturl);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    replyForecast = _nam.get(req);
    connect(replyForecast, &QNetworkReply::finished, this, &WeatherModel::onWeatherForecastRequestFinished);
    QNetworkRequest req1 = QNetworkRequest(currentturl);
    req1.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    replyCurrent = _nam.get(req1);
    connect(replyCurrent, &QNetworkReply::finished, this, &WeatherModel::onWeatherCurrentRequestFinished);
}

QString WeatherModel::cityName() const
{
    return m_cityName;
}

QString WeatherModel::countryID() const
{
    return m_countryID;
}

QHash<int, QByteArray> WeatherModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TimestampRole] = "timestamp";
    roles[TemperatureRole] = "temp";
    roles[TemperatureMinRole] = "temp_min";
    roles[TemperatureMaxRole] = "temp_max";
    roles[PressureRole] = "pressure";
    roles[PressureSeaLevelRole] = "pressure_sea_level";
    roles[PressureGroundLevelRole] = "pressure_grnd_level";
    roles[HumidityRole] = "humidity";
    roles[WeatherConditionIdRole] = "weather_codition_id";
    roles[WeatherConditionNameRole] = "weather_codition_name";
    roles[WeatherConditionDescriptionRole] = "weather_codition_description";
    roles[WeatherConditionIconIdRole] = "weather_codition_icon_id";
    roles[CloudsCoverageRole] = "clouds_coverage";
    roles[WindSpeedRole] = "wind_speed";
    roles[WindDegreesRole] = "wind_degrees";
    roles[WindGustRole] = "wind_gust";
    roles[Rain3HRole] = "rain_3h";
    roles[Snow3HRole] = "snow_3h";
    roles[TimestampStringRole] = "timestamp_string";
    return roles;
}

int WeatherModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_forecastList.size();
}

QVariant WeatherModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_forecastList.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == WeatherConditionNameRole)
        return m_forecastList.at(index.row())->weather_codition_name();
    else if (role == TimestampRole)
        return m_forecastList.at(index.row())->timestamp();
    else if (role == TemperatureRole)
        return m_forecastList.at(index.row())->temp();
    else if (role == TemperatureMinRole)
        return m_forecastList.at(index.row())->temp_min();
    else if (role == TemperatureMaxRole)
        return m_forecastList.at(index.row())->temp_max();
    else if (role == PressureRole)
        return m_forecastList.at(index.row())->pressure();
    else if (role == HumidityRole)
        return m_forecastList.at(index.row())->humidity();
    else if (role == PressureSeaLevelRole)
        return m_forecastList.at(index.row())->pressure_sea_level();
    else if (role == PressureGroundLevelRole)
        return m_forecastList.at(index.row())->pressure_grnd_level();
    else if (role == WeatherConditionIdRole)
        return m_forecastList.at(index.row())->weather_codition_id();
    else if (role == WeatherConditionDescriptionRole)
        return m_forecastList.at(index.row())->weather_codition_description();
    else if (role == WeatherConditionIconIdRole)
        return m_forecastList.at(index.row())->weather_codition_icon_id();
    else if (role == CloudsCoverageRole)
        return m_forecastList.at(index.row())->clouds_coverage();
    else if (role == WindSpeedRole)
        return m_forecastList.at(index.row())->wind_speed();
    else if (role == WindDegreesRole)
        return m_forecastList.at(index.row())->wind_degrees();
    else if (role == WindGustRole)
        return m_forecastList.at(index.row())->wind_gust();
    else if (role == Rain3HRole)
        return m_forecastList.at(index.row())->rain_3h();
    else if (role == Snow3HRole)
        return m_forecastList.at(index.row())->snow_3h();
    else if (role == TimestampStringRole)
        return m_forecastList.at(index.row())->timestamp_string();
    else
        return QVariant();
}

void WeatherModel::setCityName(QString cityName)
{
    if (m_cityName == cityName)
        return;

    m_cityName = cityName;
    emit cityNameChanged(cityName);
}

void WeatherModel::setCountryID(QString countryID)
{
    if (m_countryID == countryID)
        return;

    m_countryID = countryID;
    emit countryIDChanged(countryID);
}

int WeatherModel::measuresForDate(const QDateTime &date) const
{
    return m_forecastDates.values(date.date()).size();
}

int WeatherModel::measuresForDate(int index) const
{
    if (m_forecastDates.uniqueKeys().size() > 0 && m_forecastDates.uniqueKeys().size() > index)
        return m_forecastDates.values(m_forecastDates.uniqueKeys().at(index)).size();
    else
        return 0;
}

void WeatherModel::setCurrentWeather(WeatherData *currentWeather)
{
    if (m_currentWeather == currentWeather)
        return;

    m_currentWeather = currentWeather;
    emit currentWeatherChanged(currentWeather);
}

void WeatherModel::setDaysNumber(int daysNumber)
{
    if (m_daysNumber == daysNumber)
        return;

    m_daysNumber = daysNumber;
    //qDebug() << "days number" << daysNumber;
    emit daysNumberChanged(daysNumber);
}


void parseMainObject(const QJsonObject &obj, WeatherData *wData) {
    wData->set_temp(obj.value("temp").toDouble());
    wData->set_temp_min(obj.value("temp_min").toDouble());
    wData->set_temp_max(obj.value("temp_max").toDouble());
    wData->set_pressure(obj.value("pressure").toDouble());
    wData->set_pressure_sea_level(obj.value("sea_level").toDouble());
    wData->set_pressure_grnd_level(obj.value("grnd_level").toDouble());
    wData->set_humidity(obj.value("humidity").toDouble());
}


void parseWeatherObject(const QJsonArray &array, WeatherData *wData) {
    foreach (const QJsonValue & wvalue, array) {
        const QJsonObject &weatherObj = wvalue.toObject();
        if (!weatherObj.isEmpty()) {
            wData->set_weather_codition_name(weatherObj.value("main").toString());
            wData->set_weather_codition_id(weatherObj.value("id").toInt());
            wData->set_weather_codition_icon_id(weatherObj.value("icon").toString());
            wData->set_weather_codition_description(weatherObj.value("description").toString());
        }
        break;
    }
}

void parseWindObject(const QJsonObject &obj, WeatherData *wData) {
    wData->set_wind_speed(obj.value("speed").toDouble());
    wData->set_wind_degrees(obj.value("deg").toDouble());
}

void WeatherModel::onWeatherCurrentRequestFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError){
        QByteArray arr = reply->readAll();
        //qDebug() << reply->url() << arr;
        QJsonDocument  jsonDoc = QJsonDocument::fromJson(arr);

        QJsonObject obj = jsonDoc.object();

        int cod = obj.value("cod").toInt();
        if (cod != 200) {
            emit m_wcommon->weatherDownloadError(WeatherCommon::Current, cod);
            replyCurrent = nullptr;
            return;
        }

        m_currentWeather->set_timestamp(QDateTime::fromTime_t(obj.value("dt").toInt(), Qt::UTC, m_wcommon->getTimezoneOffset()));
        //wData->set_timestamp(QDateTime::fromString(wData->timestamp_string(), "yyyy-MM-dd HH:mm:ss"));
        QJsonObject mainObj = obj.value("main").toObject();

        if (!mainObj.isEmpty()) {
            parseMainObject(mainObj, m_currentWeather);
        }

        QJsonObject windObj = obj.value("wind").toObject();
        if (!windObj.isEmpty()) {
            parseWindObject(windObj, m_currentWeather);
        }

        QJsonObject cloudsObj = obj.value("clouds").toObject();
        if (!cloudsObj.isEmpty()) {
            m_currentWeather->set_clouds_coverage(cloudsObj.value("all").toDouble());
        }
        QJsonArray weatherarray = obj.value("weather").toArray();
        parseWeatherObject(weatherarray, m_currentWeather);

        QJsonObject rainObj = obj.value("rain").toObject();
        if (!rainObj.isEmpty()) {
            m_currentWeather->set_rain_3h(rainObj.value("3h").toDouble());
        }
        QJsonObject snowObj = obj.value("snow").toObject();
        if (!snowObj.isEmpty()) {
            m_currentWeather->set_snow_3h(snowObj.value("3h").toDouble());
        }
        m_wcommon->setBackgroundColor(m_currentWeather->temp());
        emit currentWeatherChanged(m_currentWeather);

    } else {
        m_wcommon->setBackgroundColor(0.0f);
        emit m_wcommon->weatherDownloadError(WeatherCommon::Current, reply->error());
    }
    replyCurrent = nullptr;
}

void WeatherModel::onWeatherForecastRequestFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError){
        QByteArray arr = reply->readAll();
        QJsonDocument  jsonDoc = QJsonDocument::fromJson(arr);

        QJsonObject obj = jsonDoc.object();

        int cod = obj.value("cod").toString().toInt();
        if (cod != 200) {
            qDebug() << "Forecast request failure" << cod;
            emit m_wcommon->weatherDownloadError(WeatherCommon::Forecast, cod);
            replyForecast = nullptr;
            return;
        }

        QJsonObject cityObj = obj.value("city").toObject();
        if (!cityObj.isEmpty()) {
            const QString cityName = cityObj.value("name").toString();
            if (!cityName.isEmpty()) {
                setCityName(cityName);
            }

            const QString countryID = cityObj.value("country").toString();
            if (!countryID.isEmpty()) {
                setCountryID(countryID);
            }
        }

        QJsonArray weatherList = obj.value("list").toArray();
        beginResetModel();
        qDeleteAll(m_forecastList);
        m_forecastList.clear();
        m_forecastDates.clear();

        foreach (const QJsonValue & value, weatherList) {
            QJsonObject obj1 = value.toObject();
            if (!obj1.isEmpty()) {
                WeatherData *wData = new WeatherData;
                QQmlEngine::setObjectOwnership(wData, QQmlEngine::CppOwnership);
                wData->set_timestamp(QDateTime::fromTime_t(obj1.value("dt").toInt(), Qt::UTC, m_wcommon->getTimezoneOffset()));
                wData->set_timestamp_string(obj1.value("dt_txt").toString());
                //wData->set_timestamp(QDateTime::fromString(wData->timestamp_string(), "yyyy-MM-dd HH:mm:ss"));
                QJsonObject mainObj = obj1.value("main").toObject();

                if (!mainObj.isEmpty()) {
                    parseMainObject(mainObj, wData);
                }

                QJsonObject windObj = obj1.value("wind").toObject();
                if (!windObj.isEmpty()) {
                    parseWindObject(windObj, wData);
                }

                QJsonObject cloudsObj = obj1.value("clouds").toObject();
                if (!cloudsObj.isEmpty()) {
                    wData->set_clouds_coverage(cloudsObj.value("all").toDouble());
                }
                QJsonArray weatherarray = obj1.value("weather").toArray();
                parseWeatherObject(weatherarray, wData);

                QJsonObject rainObj = obj1.value("rain").toObject();
                if (!rainObj.isEmpty()) {
                    wData->set_rain_3h(rainObj.value("3h").toDouble());
                }
                QJsonObject snowObj = obj1.value("snow").toObject();
                if (!snowObj.isEmpty()) {
                    wData->set_snow_3h(snowObj.value("3h").toDouble());
                }
                m_forecastList.append(wData);
                m_forecastDates.insertMulti(wData->timestamp().date(), wData);
                //qDebug() << wData->dump();
            }
        }
        endResetModel();
        setDaysNumber(m_forecastDates.uniqueKeys().size());
        //save last search
        m_wcommon->saveLastRequestedWeather(m_cityName + "," + m_countryID);
    } else {
        emit m_wcommon->weatherDownloadError(WeatherCommon::Forecast, reply->error());
        qDebug() << "Forecast request failure" <<reply->error();
        m_wcommon->setBackgroundColor(0.0f);
    }
    replyForecast = nullptr;
}

void WeatherModel::setUpdateInterval(int updateInterval)
{
    _updateInterval = updateInterval;
    if (updateInterval > 0) {
        _updateTimer.setInterval(updateInterval*1000);
        _updateTimer.start();
    } else {
        _updateTimer.stop();
    }
}

int WeatherModel::daysNumber() const
{
    return m_daysNumber;
}

WeatherData *WeatherModel::currentWeather() const
{
    return m_currentWeather;
}

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), _weatherDateIndex(-1) {
}

void SortFilterProxyModel::setWeatherDateIndex(int weatherIndex)
{
    _weatherDateIndex = weatherIndex;
    invalidateFilter();
    setCount(rowCount());
}

int SortFilterProxyModel::count() const
{
    return m_count;
}

void SortFilterProxyModel::setCount(int count)
{
    m_count = count;
    emit countChanged(count);
}

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    Q_UNUSED(sourceParent)

    if(_weatherDateIndex < 0)
        return true;

    WeatherModel *wm = static_cast<WeatherModel*>(sourceModel());
    if (wm == Q_NULLPTR)
        return true;

    QDate date = wm->m_forecastDates.uniqueKeys().at(_weatherDateIndex);
    return wm->m_forecastDates.values(date).contains(wm->m_forecastList.at(sourceRow));
}


#include "weathermodel.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtQml>

const QString weatherUrl = "http://api.openweathermap.org/data/2.5/forecast?";
const QString appID = "&APPID=05db5537e156973790f3845e7c1c49dc";

WeatherModel::WeatherModel(QAbstractItemModel *parent):
    QAbstractListModel(parent)
  , _latitude(13.151764)
  , _longitude(77.485829)
  , _updateInterval(-1)
  , m_daysNumber(0)
{
    qmlRegisterType<WeatherForecastData>("weathermodel", 1, 0, "WeatherForecastData");

    connect(&_updateTimer, &QTimer::timeout, this, &WeatherModel::requestWeatherUpdate);
    _updateTimer.setSingleShot(false);
}

void WeatherModel::setCoordinates(qreal latitude, qreal longitude)
{
    bool changed = false;
    if (latitude != _latitude) {
        _latitude = latitude;
        changed = true;
    }
    if (longitude != _longitude) {
        _longitude = longitude;
        changed = true;
    }
    if (changed) {
        requestWeatherUpdate();
    }
}

void WeatherModel::requestWeatherUpdate()
{
    QUrl url = QUrl(QString("%1lat=%2&lon=%3%4").arg(weatherUrl).arg(_latitude).arg(_longitude).arg(appID));

    QNetworkReply *reply = _nam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &WeatherModel::onWeatherRequestFinished);
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

void WeatherModel::setDays(QList<WeatherForecastData *> days)
{
    if (!m_days.isEmpty())
        m_days.clear();

    m_days = days;
    emit daysChanged(days);
}

int WeatherModel::measuresForDate(const QDateTime &date) const
{
    return _forecastList.values(date.date()).size();
}

int WeatherModel::measuresForDate(int index) const
{
    if (_forecastList.uniqueKeys().size() > 0 && _forecastList.uniqueKeys().size() > index)
        return _forecastList.values(_forecastList.uniqueKeys().at(index)).size();
    else
        return 0;
}

void WeatherModel::setDaysNumber(int daysNumber)
{
    if (m_daysNumber == daysNumber)
        return;

    m_daysNumber = daysNumber;
    qDebug() << "days number" << daysNumber;
    emit daysNumberChanged(daysNumber);
}

void WeatherModel::onWeatherRequestFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    if(reply->error() == QNetworkReply::NoError){
        QByteArray arr = reply->readAll();
        QJsonDocument  jsonDoc = QJsonDocument::fromJson(arr);

        QJsonObject obj = jsonDoc.object();

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
        _forecastList.clear();

        foreach (const QJsonValue & value, weatherList) {
            QJsonObject obj1 = value.toObject();
            if (!obj1.isEmpty()) {
                WeatherForecastData *wData = new WeatherForecastData;
                QQmlEngine::setObjectOwnership(wData, QQmlEngine::CppOwnership);
                //wData->set_timestamp(QDateTime::fromTime_t(obj1.value("dt").toInt()));
                wData->set_timestamp_string(obj1.value("dt_txt").toString());
                wData->set_timestamp(QDateTime::fromString(wData->timestamp_string(), "yyyy-MM-dd HH:mm:ss"));
                QJsonObject mainObj = obj1.value("main").toObject();

                if (!mainObj.isEmpty()) {
                    wData->set_temp(mainObj.value("temp").toDouble());
                    wData->set_temp_min(mainObj.value("temp_min").toDouble());
                    wData->set_temp_max(mainObj.value("temp_max").toDouble());
                    wData->set_pressure(mainObj.value("pressure").toDouble());
                    wData->set_pressure_sea_level(mainObj.value("sea_level").toDouble());
                    wData->set_pressure_grnd_level(mainObj.value("grnd_level").toDouble());
                    wData->set_humidity(mainObj.value("humidity").toDouble());
                }

                QJsonObject windObj = obj1.value("wind").toObject();
                if (!windObj.isEmpty()) {
                    wData->set_wind_speed(windObj.value("speed").toDouble());
                    wData->set_wind_degrees(windObj.value("deg").toDouble());
                }

                QJsonObject cloudsObj = obj1.value("clouds").toObject();
                if (!cloudsObj.isEmpty()) {
                    wData->set_clouds_coverage(cloudsObj.value("all").toDouble());
                }
                QJsonArray weatherarray = obj1.value("weather").toArray();
                foreach (const QJsonValue & wvalue, weatherarray) {
                    QJsonObject weatherObj = wvalue.toObject();
                    if (!weatherObj.isEmpty()) {
                        wData->set_weather_codition_name(weatherObj.value("main").toString());
                        wData->set_weather_codition_id(weatherObj.value("id").toInt());
                        wData->set_weather_codition_icon_id(weatherObj.value("icon").toString());
                        wData->set_weather_codition_description(weatherObj.value("description").toString());
                    }
                }

                QJsonObject rainObj = obj1.value("rain").toObject();
                if (!rainObj.isEmpty()) {
                    wData->set_rain_3h(rainObj.value("3h").toDouble());
                }
                QJsonObject snowObj = obj1.value("snow").toObject();
                if (!snowObj.isEmpty()) {
                    wData->set_snow_3h(snowObj.value("3h").toDouble());
                }
                m_forecastList.append(wData);
                _forecastList.insertMulti(wData->timestamp().date(), wData);
                //qDebug() << wData->dump();
            }

        }
        endResetModel();
        if (m_forecastList.size() > 0) {
            QList<WeatherForecastData*> _avgdays;
            foreach(QDate  _dt, _forecastList.uniqueKeys()) {
                const QList<WeatherForecastData*> _weatherperday = _forecastList.values(_dt);
                int avgweather = _weatherperday.size() / 2;
                //qDebug() << "append avg weather for " << _dt << avgweather;
                _avgdays.append(_weatherperday.at(avgweather));
            }
            setDays(_avgdays);
            setDaysNumber(_avgdays.size());
        }
    } else {
        qDebug() << "Failure" <<reply->errorString();
    }


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

WeatherForecastData *WeatherModel::getAverageWeather(int dayindex)
{
    if (dayindex >= 0 && m_days.size() > 0 && m_days.size() > dayindex)
        return m_days.at(dayindex);
    return Q_NULLPTR;
}

WeatherForecastData *WeatherModel::getWeather(int day, int index)
{
    if (day < 0 || _forecastList.uniqueKeys().size() < 0 || _forecastList.uniqueKeys().size() <= day ||
        _forecastList.values(_forecastList.uniqueKeys().at(day)).size() < 0 ||
        _forecastList.values(_forecastList.uniqueKeys().at(day)).size() <= index)
        return Q_NULLPTR;
    return _forecastList.values(_forecastList.uniqueKeys().at(day)).at(index);
}


SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), _weatherDateIndex(-1) {
}

void SortFilterProxyModel::setWeatherDateIndex(int weatherIndex)
{
    _weatherDateIndex = weatherIndex;
    invalidateFilter();
}

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    Q_UNUSED(sourceParent)

    if(_weatherDateIndex < 0)
        return true;

    WeatherModel *wm = static_cast<WeatherModel*>(sourceModel());
    if (wm == Q_NULLPTR)
        return true;

    QDate date = wm->_forecastList.uniqueKeys().at(_weatherDateIndex);
    return wm->_forecastList.values(date).contains(wm->m_forecastList.at(sourceRow));
}


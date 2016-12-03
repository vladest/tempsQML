#include "googlebrowserlocation.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

GoogleBrowserLocation::GoogleBrowserLocation(QObject *parent) : QObject(parent), m_tzoffset(0) {
}

QGeoCoordinate GoogleBrowserLocation::browserCoordinate() const
{
    return m_browserCoordinate;
}

void GoogleBrowserLocation::requestTimezone(const QGeoCoordinate &coord)
{
    QUrl url(QStringLiteral("https://maps.googleapis.com/maps/api/timezone/json"));
    QUrlQuery query;
    query.addQueryItem("location", QString("%1,%2").arg(coord.latitude(),0, 'f').arg(coord.longitude(),0, 'f'));
    query.addQueryItem("timestamp", QString::number(QDateTime::currentDateTime().toTime_t()/1000));
    query.addQueryItem("key", "AIzaSyCDBVNwnbTjzXCZbKdJEJmkCwVKIK-aFlo");
    url.setQuery(query);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    QNetworkReply *r = nam.get(req);
    connect(r, &QNetworkReply::finished, this, &GoogleBrowserLocation::onTimezoneFinished);
}

void GoogleBrowserLocation::requestCoordinates()
{
    QUrl url(QStringLiteral("https://maps.googleapis.com/maps/api/browserlocation/json"));
    QUrlQuery query;
    query.addQueryItem("browser", "chromium");
    query.addQueryItem("sensor", "true");
    url.setQuery(query);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    QNetworkReply *r = nam.get(req);
    connect(r, &QNetworkReply::finished, this, &GoogleBrowserLocation::onFinished);
}

int GoogleBrowserLocation::tzoffset() const
{
    return m_tzoffset;
}

void GoogleBrowserLocation::setTzoffset(int tzoffset)
{
    m_tzoffset = tzoffset;
    emit tzoffsetChanged(tzoffset);
}

void GoogleBrowserLocation::onFinished() {
    QNetworkReply *r = static_cast<QNetworkReply*>(sender());
    if (!r)
        return;
    r->deleteLater();
    if (r->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(r->readAll());
        if (!doc.isEmpty() && doc.isObject()) {
            QJsonObject o = doc.object();
            if (o.value("status").toString() == "OK") {
                QJsonObject loc = o.value("location").toObject();
                QGeoCoordinate coord;
                coord.setLatitude(loc.value("lat").toDouble());
                coord.setLongitude(loc.value("lng").toDouble());
                setBrowserCoordinate(coord);
            }
        }
    } else {
        qDebug() << "request error" << r->error() << r->url();
    }
}

void GoogleBrowserLocation::onTimezoneFinished()
{
    QNetworkReply *r = static_cast<QNetworkReply*>(sender());
    if (!r)
        return;
    r->deleteLater();
    if (r->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(r->readAll());
        if (!doc.isEmpty() && doc.isObject()) {
            QJsonObject o = doc.object();
            if (o.value("status").toString() == "OK") {
                int dstOffset = o.value("dstOffset").toInt();
                int rawOffset = o.value("rawOffset").toInt();
                setTzoffset(dstOffset + rawOffset);
            }
        }
    } else {
        qDebug() << "request error" << r->error() << r->url();
    }
}

void GoogleBrowserLocation::setBrowserCoordinate(QGeoCoordinate browserCoordinate)
{
    if (m_browserCoordinate == browserCoordinate)
        return;

    //qDebug() << "coord" << browserCoordinate;
    m_browserCoordinate = browserCoordinate;
    emit browserCoordinateChanged(browserCoordinate);
    requestTimezone(m_browserCoordinate);
}

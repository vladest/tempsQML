#ifndef GOOGLEBROWSERLOCATION_H
#define GOOGLEBROWSERLOCATION_H

#include <QObject>
#include <QtPositioning/QGeoCoordinate>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GoogleBrowserLocation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate browserCoordinate READ browserCoordinate NOTIFY browserCoordinateChanged)
    Q_PROPERTY(int tzoffset READ tzoffset WRITE setTzoffset NOTIFY tzoffsetChanged)

public:
    explicit GoogleBrowserLocation(QObject *parent = nullptr);
    QGeoCoordinate browserCoordinate() const;
    Q_INVOKABLE void requestCoordinates();
    int tzoffset() const;

signals:
    void browserCoordinateChanged(QGeoCoordinate browserCoordinate);
    void tzoffsetChanged(int tzoffset);

private slots:
    void onFinished();
    void onTimezoneFinished();
private:
    void setBrowserCoordinate(QGeoCoordinate browserCoordinate);
    void setTzoffset(int tzoffset);
    void requestTimezone(const QGeoCoordinate &coord);

    QGeoCoordinate m_browserCoordinate;
    QNetworkAccessManager nam;
    int m_tzoffset;
};

#endif // GOOGLEBROWSERLOCATION_H

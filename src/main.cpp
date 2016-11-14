#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>
#include <googlebrowserlocation.h>
#include "weathermodel.h"
#include "weatherdailymodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "127.0.0.1", 3128);
    QNetworkProxy::setApplicationProxy(proxy);
    //QNetworkProxyFactory::setUseSystemConfiguration(true);

    GoogleBrowserLocation gbl;
    WeatherModel wModel;
    WeatherDailyModel wDailyModel;

    QObject::connect(&gbl, &GoogleBrowserLocation::browserCoordinateChanged, &wModel, &WeatherModel::setCoordinates);
    QObject::connect(&gbl, &GoogleBrowserLocation::tzoffsetChanged, &wModel, &WeatherModel::setTimezoneOffset);
    QObject::connect(&gbl, &GoogleBrowserLocation::browserCoordinateChanged, &wDailyModel, &WeatherDailyModel::setCoordinates);
    QObject::connect(&gbl, &GoogleBrowserLocation::tzoffsetChanged, &wDailyModel, &WeatherDailyModel::setTimezoneOffset);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("BrowserCoordinate", &gbl);
    engine.rootContext()->setContextProperty("WeatherModel", &wModel);
    engine.rootContext()->setContextProperty("WeatherDailyModel", &wDailyModel);
    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    return app.exec();
}

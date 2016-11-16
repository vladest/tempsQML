#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>

#include <QAction>
#include <QMenu>
#include <googlebrowserlocation.h>
#include "weathermodel.h"
#include "weatherdailymodel.h"
#include "systemtrayicon.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    GoogleBrowserLocation gbl;
    WeatherModel wModel;
    WeatherDailyModel wDailyModel;

    QObject::connect(&gbl, &GoogleBrowserLocation::browserCoordinateChanged, &wModel, &WeatherModel::setCoordinates);
    QObject::connect(&gbl, &GoogleBrowserLocation::tzoffsetChanged, &wModel, &WeatherModel::setTimezoneOffset);
    QObject::connect(&gbl, &GoogleBrowserLocation::browserCoordinateChanged, &wDailyModel, &WeatherDailyModel::setCoordinates);
    QObject::connect(&gbl, &GoogleBrowserLocation::tzoffsetChanged, &wDailyModel, &WeatherDailyModel::setTimezoneOffset);

    QQmlApplicationEngine engine;

    SystemTrayIcon *trayIcon = new SystemTrayIcon(engine.rootContext());
    trayIcon->setIcon(QIcon(":/qml/images/app.png"));
    trayIcon->show();
    engine.rootContext()->setContextProperty("SystemTrayIcon", trayIcon);
    engine.rootContext()->setContextProperty("BrowserCoordinate", &gbl);
    engine.rootContext()->setContextProperty("WeatherModel", &wModel);
    engine.rootContext()->setContextProperty("WeatherDailyModel", &wDailyModel);
    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    return app.exec();
}

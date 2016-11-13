#ifndef WEATHERDAILYMODEL_H
#define WEATHERDAILYMODEL_H

#include <QObject>

class WeatherDailyModel : public QObject
{
    Q_OBJECT
public:
    explicit WeatherDailyModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // WEATHERDAILYMODEL_H
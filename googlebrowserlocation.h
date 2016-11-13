#ifndef GOOGLEBROWSERLOCATION_H
#define GOOGLEBROWSERLOCATION_H

#include <QObject>

class GoogleBrowserLocation : public QObject
{
    Q_OBJECT
public:
    explicit GoogleBrowserLocation(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GOOGLEBROWSERLOCATION_H
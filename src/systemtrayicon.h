#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QtWidgets/QSystemTrayIcon>


class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    SystemTrayIcon(QObject * parent = nullptr);
public slots:
    void setNewIcon(const QString &iconPath);
};

#endif // SYSTEMTRAYICON_H

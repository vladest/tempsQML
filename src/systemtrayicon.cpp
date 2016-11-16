#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent) : QSystemTrayIcon(parent)
{
}

void SystemTrayIcon::setNewIcon(const QString &iconPath) {
    setIcon(QIcon(":/qml/images/" + iconPath));
}

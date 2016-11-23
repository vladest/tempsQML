#include "systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(QObject *parent) : QSystemTrayIcon(parent)
  , m_available(QSystemTrayIcon::isSystemTrayAvailable())
{
    if (m_available) {
        setIcon(QIcon(":/qml/images/app.png"));
        show();
    }
    emit availableChanged(m_available);
}

bool SystemTrayIcon::available() const
{
    return m_available;
}

void SystemTrayIcon::setNewIcon(const QString &iconPath) {
    if (m_available) {
        setIcon(QIcon(":/qml/images/" + iconPath));
    }
}

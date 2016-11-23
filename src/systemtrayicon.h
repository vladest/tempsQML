#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QtWidgets/QSystemTrayIcon>


class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

    Q_PROPERTY(bool available READ available NOTIFY availableChanged)

public:
    SystemTrayIcon(QObject * parent = nullptr);
    bool available() const;

public slots:
    void setNewIcon(const QString &iconPath);

signals:
    void availableChanged(bool available);

private:
    bool m_available;
};

#endif // SYSTEMTRAYICON_H

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaType>
#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0) : QSettings(QSettings::IniFormat,
                                                       QSettings::UserScope,
                                                       QCoreApplication::instance()->organizationName(),
                                                       QCoreApplication::instance()->applicationName(),
                                                       parent) {}

    Q_INVOKABLE inline void setValue(const QString &key, const QVariant &value, const QString &group = QString()) {
        QSettings settings;
        if (!group.isEmpty())
            settings.beginGroup(group);
        if (key.isEmpty() && value.toString().isEmpty())
            settings.remove("");
        settings.setValue(key, value);
        if (!group.isEmpty())
            settings.endGroup();
    }
    Q_INVOKABLE inline QVariant value(const QString &key, const QVariant &defaultValue = QVariant(), const QString &group = QString()) const {
        QSettings settings;
        if (!group.isEmpty())
            settings.beginGroup(group);
        QVariant v = settings.value(key, defaultValue);
        if (!group.isEmpty())
            settings.endGroup();
        return v;
    }
};

Q_DECLARE_METATYPE(Settings*)

#endif // SETTINGS_H


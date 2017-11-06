/**
 * @brief Common application settings
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>

#include "o3d/studio/common/settings.h"

using namespace o3d::studio::common;


Settings::Settings() :
    QObject()
{

}

Settings::~Settings()
{

}

void Settings::loadGroup(const QString &group)
{
    QRegExp re = QRegExp("([A-Z])([a-z]*)");

    QString prefix = "o3s::" + group.toLower();

    // main group settings
    QSettings lSettings(SETTINGS_FILE, QSettings::IniFormat, QCoreApplication::instance());
    lSettings.beginGroup(group);

    QString key, s;
    foreach (key, lSettings.childKeys()) {
        s = key;
        s = prefix + s.replace(re, "::\\1\\2").toLower();
        m_settings[s] = lSettings.value(key);
    }
}

void Settings::loadAll()
{
    loadGroup("Main");
    loadGroup("Plugin");
    loadGroup("Misc");
}

void Settings::saveGroup(const QString &group)
{
    QSettings lSettings(SETTINGS_FILE, QSettings::IniFormat, QCoreApplication::instance());
    lSettings.beginGroup(group);

    QString prefix = "o3s::" + group.toLower() + "::";

    QString key, s;
    QStringList sl;
    for (auto cit = m_settings.cbegin(); cit != m_settings.cend(); ++cit) {
        if (cit.key().startsWith(prefix)) {
            sl = cit.key().split("::");
            s = "";
            for (int i = 2; i < sl.length(); ++i) {
                s.append(sl[i][0].toUpper()).append(sl[i].mid(1));
            }
            lSettings.setValue(s, cit.value());
        }
    }
}

void Settings::saveAll()
{
    saveGroup("Main");
    saveGroup("Plugin");
    saveGroup("Misc");
}

void Settings::set(const QString &key, const QVariant &value, bool force)
{
    auto it = m_settings.find(key);
    if (it != m_settings.end()) {
        if (force || it.value() != value) {
            m_settings[key] = value;

            // emit
            emit settingChanged(key, value);
        }
    } else {
        m_settings[key] = value;

        // emit
        emit settingChanged(key, value);
    }
}

QVariant Settings::get(const QString &key, const QVariant& _default)
{
    auto cit = m_settings.find(key);
    if (cit != m_settings.cend()) {
        // get current
        return cit.value();
    } else {
        // set to default and returns
        m_settings[key] = _default;
        return _default;
    }
}

bool Settings::has(const QString &key) const
{
    auto cit = m_settings.find(key);
    return cit != m_settings.cend();
}

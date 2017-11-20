/**
 * @brief Common application settings
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>

#include "o3d/studio/common/settings.h"

using namespace o3d::studio::common;


Settings::Settings() :
    BaseObject()
{

}

Settings::~Settings()
{

}

void Settings::loadGroup(const String &group)
{
    QRegExp re = QRegExp("([A-Z])([a-z]*)");

    QString prefix = "o3s::" + toQString(group).toLower();

    // main group settings
    QSettings lSettings(SETTINGS_FILE, QSettings::IniFormat, QCoreApplication::instance());
    lSettings.beginGroup(toQString(group));

    QString key, s;
    foreach (key, lSettings.childKeys()) {
        s = key;
        s = prefix + s.replace(re, "::\\1\\2").toLower();
        m_settings[fromQString(s)] = lSettings.value(key);
    }
}

void Settings::loadAll()
{
    loadGroup("Main");
    loadGroup("Plugin");
    loadGroup("Misc");
    loadGroup("Common");
}

void Settings::saveGroup(const String &group)
{
    QSettings lSettings(SETTINGS_FILE, QSettings::IniFormat, QCoreApplication::instance());
    lSettings.beginGroup(toQString(group));

    String prefix = "o3s::" + group.toLower() + "::";

    QString key, s;
    QStringList sl;
    for (auto cit = m_settings.cbegin(); cit != m_settings.cend(); ++cit) {
        if (cit->first.startsWith(prefix)) {
            sl = toQString(cit->first).split("::");
            s = "";
            for (int i = 2; i < sl.length(); ++i) {
                s.append(sl[i][0].toUpper()).append(sl[i].mid(1));
            }
            lSettings.setValue(s, cit->second);
        }
    }
}

void Settings::saveAll()
{
    saveGroup("Main");
    saveGroup("Plugin");
    saveGroup("Misc");
    saveGroup("Common");
}

void Settings::set(const String &key, const QVariant &value, Bool force)
{
    auto it = m_settings.find(key);
    if (it != m_settings.end()) {
        if (force || it->second != value) {
            m_settings[key] = value;

            // emit
            settingChanged(key, value);
        }
    } else {
        m_settings[key] = value;

        // emit
        settingChanged(key, value);
    }
}

QVariant Settings::get(const String &key, const QVariant& _default)
{
    auto cit = m_settings.find(key);
    if (cit != m_settings.cend()) {
        // get current
        return cit->second;
    } else {
        // set to default and returns
        m_settings[key] = _default;
        return _default;
    }
}

o3d::Bool Settings::has(const String &key) const
{
    auto cit = m_settings.find(key);
    return cit != m_settings.cend();
}

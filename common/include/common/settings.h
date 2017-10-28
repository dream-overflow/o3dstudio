/**
 * @brief Common application settings
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_SETTINGS_H
#define _O3DS_COMMON_SETTINGS_H

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace o3d {
namespace studio {
namespace common {

static const char PLUGIN_PATH[] = "plugins";
static const char SETTINGS_FILE[] = "o3dstudiosettings.ini";
static const char LANGUAGES_PATH[] = "languages";

#ifdef _MSC_VER
static const char MODULE_EXT[] = "*.dll";
#else
static const char MODULE_EXT[] = "*.so";
#endif

class Settings : public QObject
{
    Q_OBJECT

public:

    Settings();
    virtual ~Settings();

    void loadAll();
    void saveAll();

    void set(const QString &key, const QVariant& value, bool force = false);
    QVariant get(const QString &key, const QVariant& _default = QVariant());

    bool has(const QString &key) const;

signals:

    void settingChanged(const QString &key, const QVariant &value);

private:

    QMap<QString, QVariant> m_settings;

    //! Non copyable
    Settings(const Settings&);

    //! Non assignable
    void operator=(const Settings&);

    void loadGroup(const QString &group);
    void saveGroup(const QString &group);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SETTINGS_H

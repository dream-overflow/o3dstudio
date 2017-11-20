/**
 * @brief Common application settings
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_SETTINGS_H
#define _O3DS_COMMON_SETTINGS_H

#include <QtCore/QVariant>
#include <o3d/core/baseobject.h>

#include "global.h"

#include <map>

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

class O3S_API Settings : public BaseObject
{
public:

    Settings();
    virtual ~Settings();

    void loadAll();
    void saveAll();

    void set(const String &key, const QVariant& value, Bool force = false);
    QVariant get(const String &key, const QVariant& _default = QVariant());

    Bool has(const String &key) const;

public /*signals*/:

    Signal<const String &/*key*/, const QVariant &/*value*/> settingChanged{this};

private:

    std::map<String, QVariant> m_settings;

    //! Non copyable
    Settings(const Settings&);

    //! Non assignable
    void operator=(const Settings&);

    void loadGroup(const String &group);
    void saveGroup(const String &group);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SETTINGS_H

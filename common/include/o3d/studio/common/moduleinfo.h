/**
 * @brief Common module info object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_MODULEINFO_H
#define _O3DS_COMMON_MODULEINFO_H

#include <map>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API ModuleInfo
{
public:

    ModuleInfo(const String &name, const std::map<String, String> &properties);
    ~ModuleInfo();

    const String &name() const;
    String verbose() const;
    String version() const;
    String vendor() const;
    String description() const;
    String section() const;

    String property(const String &name, const String &defaut = "") const;

    const std::map<String, String> properties() const;

protected:

    std::map<String, String> m_properties;
    String m_name;               //!< Unique module name
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MODULEINFO_H

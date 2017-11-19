/**
 * @brief Common module info
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include "o3d/studio/common/moduleinfo.h"

using namespace o3d::studio::common;

ModuleInfo::ModuleInfo(const String &name, const std::map<String, String> &properties) :
    m_properties(properties),
    m_name(name)
{
}

ModuleInfo::~ModuleInfo()
{

}

const o3d::String &ModuleInfo::name() const
{
    return m_name;
}

o3d::String ModuleInfo::verbose() const
{
    return property("verbose", m_name);
}

o3d::String ModuleInfo::version() const
{
    return property("version", "0.0.1");
}

o3d::String ModuleInfo::vendor() const
{
    return property("vendor", "Unknown vendor");
}

o3d::String ModuleInfo::description() const
{
    return property("description", "");
}

o3d::String ModuleInfo::section() const
{
    // default to o3dstudio section
    return property("section", "o3s::plugin::o3dstudio");
}

o3d::String ModuleInfo::property(const String &name, const String &defaut) const
{
    auto cit = m_properties.find(name);
    if (cit != m_properties.end()) {
        return cit->second;
    }

    return defaut;
}

const std::map<o3d::String, o3d::String> ModuleInfo::properties() const
{
    return m_properties;
}

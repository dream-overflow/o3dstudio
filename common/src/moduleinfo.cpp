/**
 * @brief Common module info
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include "o3d/studio/common/moduleinfo.h"

using namespace o3d::studio::common;

ModuleInfo::ModuleInfo(const QString &name, const QMap<QString, QVariant> &properties) :
    m_properties(properties),
    m_name(name)
{
}

ModuleInfo::~ModuleInfo()
{

}

const QString &ModuleInfo::name() const
{
    return m_name;
}

QString ModuleInfo::verbose() const
{
    return property("verbose", QString(m_name)).toString();
}

QString ModuleInfo::version() const
{
    return property("version", QString("0.0.1")).toString();
}

QString ModuleInfo::vendor() const
{
    return property("vendor", QString("Unknown vendor")).toString();
}

QString ModuleInfo::description() const
{
    return property("vendor", QString("")).toString();
}

QString ModuleInfo::section() const
{
    // default to o3dstudio section
    return property("section", QString("o3s::plugin::o3dstudio")).toString();
}

QVariant ModuleInfo::property(const QString &name, const QVariant &defaut) const
{
    auto cit = m_properties.find(name);
    if (cit != m_properties.end()) {
        return cit.value();
    }

    return defaut;
}

const QMap<QString, QVariant> ModuleInfo::properties() const
{
    return m_properties;
}

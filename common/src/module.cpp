/**
 * @brief Common module
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#include <o3d/core/dynamiclibrary.h>

#include "o3d/studio/common/module.h"

using namespace o3d::studio::common;

Module::Module(const String &name) :
    m_name(name)
{

}

Module::~Module()
{

}

const o3d::String &Module::name() const
{
    return m_name;
}

o3d::Bool Module::started() const
{
    return m_started;
}

ModuleInfo Module::provideInfo() const
{
    return ModuleInfo(m_name, m_properties);
}

o3d::DynamicLibrary *Module::library()
{
    return nullptr;
}

const o3d::BitSet64& Module::capacities() const
{
    return m_capacities;
}

o3d::Bool Module::hasCapacity(Capacity cap) const
{
    return m_capacities.getBit(cap);
}

o3d::Bool Module::start()
{
    return false;
}

o3d::Bool Module::stop()
{
    m_report.clear();
    return false;
}

const o3d::T_StringList &Module::report() const
{
    return m_report;
}

o3d::Bool Module::restart()
{
    if (m_started) {
        if (!stop()) {
            return False;
        }
    }

    if (!m_started) {
        if (!start()) {
            return False;
        }
    }

    return m_started;
}

Importer* Module::importer()
{
    return nullptr;
}

Builder* Module::builder()
{
    return nullptr;
}

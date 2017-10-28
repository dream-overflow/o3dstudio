/**
 * @brief Common module
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#include <QtCore/QLibrary>

#include "common/module.h"

using namespace o3d::studio::common;

Module::Module(const QString &name) :
    m_name(name)
{

}

Module::~Module()
{

}

const QString &Module::name() const
{
    return m_name;
}

bool Module::started() const
{
    return m_started;
}

ModuleInfo Module::provideInfo() const
{
    return ModuleInfo(m_name, m_properties);
}

QLibrary *Module::library()
{
    return nullptr;
}

const QBitArray& Module::capacities() const
{
    return m_capacities;
}

bool Module::hasCapacity(Capacity cap) const
{
    return m_capacities.testBit(cap);
}

bool Module::start()
{
    return false;
}

bool Module::stop()
{
    m_report.clear();
    return false;
}

const QStringList &Module::report() const
{
    return m_report;
}

bool Module::restart()
{
    if (m_started) {
        if (!stop()) {
            return false;
        }
    }

    if (!m_started) {
        if (!start()) {
            return false;
        }
    }

    return m_started;
}

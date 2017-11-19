/**
 * @brief Common dynamic module
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/dynamicmodule.h"

using namespace o3d::studio::common;

DynamicModule::DynamicModule(const String &name, DynamicLibrary *library) :
    Module(name),
    m_library(library)
{
}

DynamicModule::~DynamicModule()
{
}

o3d::Bool DynamicModule::start()
{
    return False;
}

o3d::Bool DynamicModule::stop()
{
    return False;
}

o3d::String DynamicModule::fileName() const
{
    return m_library->getName();
}

o3d::DynamicLibrary *DynamicModule::library()
{
    return m_library;
}

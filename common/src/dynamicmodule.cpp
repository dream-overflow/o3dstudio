/**
 * @brief Common dynamic module
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "common/dynamicmodule.h"

using namespace o3d::studio::common;

DynamicModule::DynamicModule(const QString &name, QLibrary *library) :
    Module(name),
    m_library(library)
{
}

DynamicModule::~DynamicModule()
{
}

bool DynamicModule::start()
{
    return false;
}

bool DynamicModule::stop()
{
    return false;
}

QString DynamicModule::getFileName() const
{
    return m_library->fileName();
}

QLibrary *DynamicModule::library()
{
    return m_library;
}

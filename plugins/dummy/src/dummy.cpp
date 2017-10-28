/**
 * @brief Dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "dummy.h"

using namespace o3d::studio::dummy;

extern "C"
{

o3d::studio::common::Module* o3dstudioPlugin(const QString &name, QLibrary *library)
{
    Dummy *module = new Dummy(name, library);
    return module;
}

} // extern "C"

Dummy::Dummy(const QString &name, QLibrary *library) :
    DynamicModule(name, library)
{
    m_properties["section"] = QString("o3s::plugin::o3dstudio");
    m_properties["verbose"] = QString("Dummy");

    m_properties["vendor"] = QString("Dream Overflow");
    m_properties["group"] = QString("Build system");
    m_properties["url"] = QString("http://o3d.dreamoverflow.com");
    m_properties["plateforms"] = QStringList({QString("all")});

    m_properties["version"] = QString("1.0.0");
    m_properties["description"] = QString("A simple dummy extension that do nothing, except to be !");
    m_properties["copyright"] = QString("(C) 2017 Dream Overflow");
    m_properties["license"] = QString("Proprietary");

    m_properties["dependencies"] = QStringList();
}

Dummy::~Dummy()
{

}

bool Dummy::start()
{
    m_started = true;
    return true;
}

bool Dummy::stop()
{
    m_started = false;
    return true;
}

/**
 * @brief Dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include "dummy.h"

using namespace o3d::studio::dummy;

extern "C"
{

o3d::studio::common::Module* o3dstudioPlugin(const o3d::String &name, o3d::DynamicLibrary *library)
{
    Dummy *module = new Dummy(name, library);
    return module;
}

} // extern "C"

Dummy::Dummy(const String &name, DynamicLibrary *library) :
    DynamicModule(name, library)
{
    m_properties["section"] = "o3s::plugin::o3dstudio";
    m_properties["verbose"] = "Dummy";

    m_properties["vendor"] = "Dream Overflow";
    m_properties["group"] = "Build system";
    m_properties["url"] = "http://o3dstudio.dreamoverflow.org";
    m_properties["plateforms"] = "all";

    m_properties["version"] = "1.0.0";
    m_properties["description"] = "A simple dummy extension that do nothing, except to be !";
    m_properties["copyright"] = "(C) 2017 Dream Overflow";
    m_properties["license"] = "Proprietary";

    m_properties["dependencies"] = "";
}

Dummy::~Dummy()
{

}

o3d::Bool Dummy::start()
{
    m_started = True;
    return True;
}

o3d::Bool Dummy::stop()
{
    m_started = False;
    return False;
}

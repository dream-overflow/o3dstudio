/**
 * @brief Common dynamic module object specialization
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_DYNAMICMODULE_H
#define _O3DS_COMMON_DYNAMICMODULE_H

#include <o3d/core/dynamiclibrary.h>

#include "module.h"
#include "global.h"

#if (defined(O3D_UNIX) || defined(O3D_MACOSX) || defined(SWIG))
  #if __GNUC__ >= 4
    #define O3S_PLUGIN_API __attribute__ ((visibility ("default")))
    #define O3S_PLUGIN_API_PRIVATE __attribute__ ((visibility ("hidden")))
    #define O3S_PLUGIN_API_TEMPLATE
  #else
    #define O3S_PLUGIN_API
    #define O3S_PLUGIN_API_PRIVATE
    #define O3S_PLUGIN_API_TEMPLATE
  #endif
#elif defined(O3D_WINDOWS)
#define O3S_PLUGIN_API __declspec(dllexport)
#define O3S_PLUGIN_API_PRIVATE
#define O3S_PLUGIN_API_TEMPLATE
#endif

namespace o3d {
namespace studio {
namespace common {

typedef Module* (*PluginFunction)(const String &, DynamicLibrary *library);

class O3S_API DynamicModule : public Module
{
public:

    DynamicModule(const String &name, DynamicLibrary *library);
    virtual ~DynamicModule();

    virtual Bool start();
    virtual Bool stop();

    String fileName() const;

    virtual DynamicLibrary* library();

private:

    DynamicLibrary *m_library;           //!< Related library instance
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DYNAMICMODULE_H

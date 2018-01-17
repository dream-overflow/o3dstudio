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

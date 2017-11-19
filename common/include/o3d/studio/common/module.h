/**
 * @brief Common module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_MODULE_H
#define _O3DS_COMMON_MODULE_H

#include <o3d/core/templatebitset.h>
#include <o3d/core/string.h>
#include <o3d/core/stringlist.h>

#include "global.h"
#include "capacity.h"
#include "moduleinfo.h"

namespace o3d {

class DynamicLibrary;

namespace studio {
namespace common {

class O3S_API Module
{
public:

    Module(const String &name);
    virtual ~Module();

    const String& name() const;
    Bool started() const;

    ModuleInfo provideInfo() const;

    virtual DynamicLibrary* library();

    const BitSet64& capacities() const;

    Bool hasCapacity(Capacity cap) const;

    virtual Bool start();
    virtual Bool stop();

    const T_StringList& report() const;

    /**
     * @brief Restart the module if loaded, and previously started.
     * @return
     */
    bool restart();

protected:

    String m_name;                //!< Unique module name
    std::map<String, String> m_properties;   //!< Module info and user properties

    BitSet64 m_capacities;        //!< List of exposed and supported capacities

    Bool m_started{false};        //!< Started module capacities

    T_StringList m_report;        //!< report of starting module
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MODULE_H

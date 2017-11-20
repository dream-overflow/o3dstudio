/**
 * @brief Common type(ref) registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_TYPEREGISTRY_H
#define _O3DS_COMMON_TYPEREGISTRY_H

#include <map>

#include <o3d/core/baseobject.h>

#include "global.h"
#include "objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API TypeRegistry : public BaseObject
{
public:

    TypeRegistry(BaseObject *parent = nullptr);
    virtual ~TypeRegistry();

    Bool registerType(EntityBaseType baseType, const String &name);

    Bool unregisterType(const String &name);
    Bool unregisterType(UInt32 id);
    Bool unregisterType(const TypeRef &typeRef);

    TypeRef typeRef(const String &name) const;

    /**
     * @brief Return the base type for a type ref
     */
    TypeRef baseTypeRef(const TypeRef &typeRef) const;

    /**
     * @brief Return the base type for a type ref id
     */
    TypeRef baseTypeRef(UInt32 id) const;

public /*signals*/:

    Signal<TypeRef> typeRegistered{this};

protected:

    UInt32 m_nextId;

    std::map<String, TypeRef> m_typeRefs;
    std::map<UInt32, TypeRef> m_typeRefsById;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TYPEREGISTRY_H

/**
 * @brief Common type(ref) registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/typeregistry.h"

using namespace o3d::studio::common;


TypeRegistry::TypeRegistry(BaseObject *parent) :
    BaseObject(parent),
    m_nextId(1)
{
    // reserved base types
    m_typeRefs[PROJECT_TYPE_STRING] = TypeRef::project();
    m_typeRefs[HUB_TYPE_STRING] = TypeRef::hub();
    m_typeRefs[FRAGMENT_TYPE_STRING] = TypeRef::fragment();
    m_typeRefs[RESOURCE_TYPE_STRING] = TypeRef::resource();
    m_typeRefs[ASSET_TYPE_STRING] = TypeRef::asset();
    m_typeRefs[COMPONENT_TYPE_STRING] = TypeRef::component();
    m_typeRefs[IMPORTER_TYPE_STRING] = TypeRef::importer();
    m_typeRefs[BUILDER_TYPE_STRING] = TypeRef::builder();

    m_typeRefsById[TypeRef::project().id()] = TypeRef::project();
    m_typeRefsById[TypeRef::hub().id()] = TypeRef::hub();
    m_typeRefsById[TypeRef::fragment().id()] = TypeRef::fragment();
    m_typeRefsById[TypeRef::resource().id()] = TypeRef::resource();
    m_typeRefsById[TypeRef::component().id()] = TypeRef::component();
    m_typeRefsById[TypeRef::importer().id()] = TypeRef::importer();
    m_typeRefsById[TypeRef::builder().id()] = TypeRef::builder();

    m_nextId = MAX_TYPE_ID + 1;

    // register the root hub because it is unique and static and no have component
    registerType(HUB_TYPE_ID, "o3s::common::hub::root");
}

TypeRegistry::~TypeRegistry()
{

}

o3d::Bool TypeRegistry::registerType(EntityBaseType baseType, const String &name)
{
    // unknown base type
    if (baseType < MIN_TYPE_ID || baseType > MAX_TYPE_ID) {
        return False;
    }

    // named already used or type already defined
    if (m_typeRefs.find(name) != m_typeRefs.end()) {
        return False;
    }

    TypeRef typeRef = TypeRef(baseType, m_nextId++, name);
    m_typeRefs[name] = typeRef;
    m_typeRefsById[typeRef.id()] = typeRef;

    typeRegistered(typeRef);
    return True;
}

o3d::Bool TypeRegistry::unregisterType(const String &name)
{
    auto it = m_typeRefs.find(name);
    auto it2 = m_typeRefsById.find(it->second.id());

    if (it != m_typeRefs.end()) {
        m_typeRefs.erase(it);
    }

    if (it2 != m_typeRefsById.end()) {
        m_typeRefsById.erase(it2);
    }

    if (it == m_typeRefs.end() || it2 == m_typeRefsById.end()) {
        return False;
    }

    return True;
}

o3d::Bool TypeRegistry::unregisterType(UInt32 id)
{
    auto it2 = m_typeRefsById.find(id);
    if (it2 != m_typeRefsById.end()) {
        m_typeRefsById.erase(it2);
    }

    // now we have the name
    auto it = m_typeRefs.find(it2->second.name());
    if (it != m_typeRefs.end()) {
        m_typeRefs.erase(it);
    }

    if (it == m_typeRefs.end() || it2 == m_typeRefsById.end()) {
        return False;
    }

    return True;
}

o3d::Bool TypeRegistry::unregisterType(const TypeRef &typeRef)
{
    auto it = m_typeRefs.find(typeRef.name());
    auto it2 = m_typeRefsById.find(typeRef.id());

    if (it != m_typeRefs.end()) {
        m_typeRefs.erase(it);
    }

    if (it2 != m_typeRefsById.end()) {
        m_typeRefsById.erase(it2);
    }

    if (it == m_typeRefs.end() || it2 == m_typeRefsById.end()) {
        return False;
    }

    return True;
}

TypeRef TypeRegistry::typeRef(const String &name) const
{
    auto cit = m_typeRefs.find(name);
    if (cit != m_typeRefs.cend()) {
        return cit->second;
    } else {
        return TypeRef();
    }
}

TypeRef TypeRegistry::baseTypeRef(const TypeRef &typeRef) const
{
    if (typeRef.isValid()) {
        // direct by using baseType()
        auto it2 = m_typeRefsById.find(typeRef.baseTypeId());
        if (it2 != m_typeRefsById.cend()) {
            return it2->second;
        }
    }

    return TypeRef();
}

TypeRef TypeRegistry::baseTypeRef(UInt32 id) const
{
    if (id > 0) {
        UInt32 baseTypeId = ((id & 0x0f) << 4) | (id & 0x0f);

        auto it2 = m_typeRefsById.find(baseTypeId);
        if (it2 != m_typeRefsById.cend()) {
            return it2->second;
        }
    }

    return TypeRef();
}

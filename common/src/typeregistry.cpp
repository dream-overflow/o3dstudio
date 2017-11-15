/**
 * @brief Common type(ref) registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/typeregistry.h"

using namespace o3d::studio::common;


TypeRegistry::TypeRegistry(QObject *parent) :
    QObject(parent),
    m_nextId(1)
{
    // reserved base types
    m_typeRefs[PROJECT_TYPE_STRING] = TypeRef::project();
    m_typeRefs[HUB_TYPE_STRING] = TypeRef::hub();
    m_typeRefs[FRAGMENT_TYPE_STRING] = TypeRef::fragment();
    m_typeRefs[ASSET_TYPE_STRING] = TypeRef::asset();
    m_typeRefs[COMPONENT_TYPE_STRING] = TypeRef::component();

    m_typeRefsById[TypeRef::project().id()] = TypeRef::project();
    m_typeRefsById[TypeRef::hub().id()] = TypeRef::hub();
    m_typeRefsById[TypeRef::fragment().id()] = TypeRef::fragment();
    m_typeRefsById[TypeRef::component().id()] = TypeRef::component();

    m_nextId = MAX_TYPE_ID + 1;
}

TypeRegistry::~TypeRegistry()
{

}

bool TypeRegistry::registerType(EntityBaseType baseType, const QString &name)
{
    // unknown base type
    if (baseType < MIN_TYPE_ID || baseType > MAX_TYPE_ID) {
        return false;
    }

    // named already used or type already defined
    if (m_typeRefs.find(name) != m_typeRefs.end()) {
        return false;
    }

    TypeRef typeRef = TypeRef(baseType, m_nextId++, name);
    m_typeRefs[name] = typeRef;
    m_typeRefsById[typeRef.id()] = typeRef;

    emit typeRegistered(typeRef);
    return true;
}

bool TypeRegistry::unregisterType(const QString &name)
{
    auto it = m_typeRefs.find(name);
    auto it2 = m_typeRefsById.find(it.value().id());

    if (it != m_typeRefs.end()) {
        m_typeRefs.erase(it);
    }

    if (it2 != m_typeRefsById.end()) {
        m_typeRefsById.erase(it2);
    }

    if (it == m_typeRefs.end() || it2 == m_typeRefsById.end()) {
        return false;
    }

    return true;
}

bool TypeRegistry::unregisterType(quint32 id)
{
    auto it2 = m_typeRefsById.find(id);
    if (it2 != m_typeRefsById.end()) {
        m_typeRefsById.erase(it2);
    }

    // now we have the name
    auto it = m_typeRefs.find(it2.value().name());
    if (it != m_typeRefs.end()) {
        m_typeRefs.erase(it);
    }

    if (it == m_typeRefs.end() || it2 == m_typeRefsById.end()) {
        return false;
    }

    return true;
}

bool TypeRegistry::unregisterType(const TypeRef &typeRef)
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
        return false;
    }

    return true;
}

TypeRef TypeRegistry::typeRef(const QString &name) const
{
    auto it = m_typeRefs.constFind(name);
    if (it != m_typeRefs.cend()) {
        return it.value();
    } else {
        return TypeRef();
    }
}

TypeRef TypeRegistry::baseTypeRef(const TypeRef &typeRef) const
{
    if (typeRef.isValid()) {
        // direct by using baseType()
        auto it2 = m_typeRefsById.constFind(typeRef.baseTypeId());
        if (it2 != m_typeRefsById.cend()) {
            return it2.value();
        }
    }

    return TypeRef();
}

TypeRef TypeRegistry::baseTypeRef(quint32 id) const
{
    if (id > 0) {
        quint32 baseTypeId = ((id & 0x0f) << 4) | (id & 0x0f);

        auto it2 = m_typeRefsById.constFind(baseTypeId);
        if (it2 != m_typeRefsById.cend()) {
            return it2.value();
        }
    }

    return TypeRef();
}

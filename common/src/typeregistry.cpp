/**
 * @brief Common type(ref) registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/typeregistry.h"

using namespace o3d::studio::common;


TypeRegistry::TypeRegistry(QObject *parent) :
    QObject(parent),
    m_nextId(1)
{
    // reserved
    m_typeRefs[PROJECT_TYPE_STRING] = TypeRef::project();
    m_typeRefs[HUB_TYPE_STRING] = TypeRef::hub();
    m_typeRefs[FRAGMENT_TYPE_STRING] = TypeRef::fragment();
    m_typeRefs[ASSET_TYPE_STRING] = TypeRef::asset();

    m_typeRefsById[PROJECT_TYPE_ID] = TypeRef::project();
    m_typeRefsById[HUB_TYPE_ID] = TypeRef::hub();
    m_typeRefsById[FRAGMENT_TYPE_ID] = TypeRef::fragment();
    m_typeRefsById[ASSET_TYPE_ID] = TypeRef::asset();

    m_nextId = 1024;
}

TypeRegistry::~TypeRegistry()
{

}

bool TypeRegistry::registerType(const TypeRef &baseType, const QString &name)
{
    // invalid base type
    if (!baseType.isValid()) {
        return false;
    }

    // unknown base type
    if (baseType.id() < PROJECT_TYPE_ID || baseType.id() > ASSET_TYPE_ID) {
        return false;
    }

    // named already used or type already defined
    if (m_typeRefs.find(name) != m_typeRefs.end()) {
        return false;
    }

    TypeRef typeRef = TypeRef(baseType.id(), m_nextId++, name);
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

bool TypeRegistry::unregisterType(qint64 id)
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
        auto it2 = m_typeRefsById.constFind(typeRef.baseType());
        if (it2 != m_typeRefsById.cend()) {
            return it2.value();
        }
    }

    return TypeRef();
}

TypeRef TypeRegistry::baseTypeRef(qint64 type) const
{
    if (type > 0) {
        // two step, find type, and then get its base type
        auto it2 = m_typeRefsById.constFind(type);
        if (it2 != m_typeRefsById.cend()) {
            auto it3 = m_typeRefsById.constFind(it2.value().baseType());
            if (it3 != m_typeRefsById.cend()) {
                return it3.value();
            }
        }
    }

    return TypeRef();
}

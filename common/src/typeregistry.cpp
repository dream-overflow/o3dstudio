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
    m_typeRefs["o3s::project"] = TypeRef(1, "o3s::project");
    m_typeRefs["o3s::fragment"] = TypeRef(2, "o3s::fragment");
    m_typeRefs["o3s::hub"] = TypeRef(3, "o3s::hub");

    m_nextId = 4;
}

TypeRegistry::~TypeRegistry()
{

}

bool TypeRegistry::registerType(const QString &name)
{
    if (m_typeRefs.find(name) != m_typeRefs.end()) {
        return false;
    }

    TypeRef typeRef = TypeRef(m_nextId++, name);
    m_typeRefs[name] = typeRef;

    emit typeRegistered(typeRef);
    return true;
}

bool TypeRegistry::unregisterType(const QString &name)
{
    auto it = m_typeRefs.find(name);

    if (it == m_typeRefs.end()) {
        return false;
    }

    m_typeRefs.erase(it);
    return true;
}

bool TypeRegistry::unregisterType(qint64 id)
{
    for (auto it = m_typeRefs.begin(); it != m_typeRefs.end(); ++it) {
        if (it.value().id() == id) {
            m_typeRefs.erase(it);
            return true;
        }
    }

    return false;
}

bool TypeRegistry::unregisterType(const TypeRef &typeRef)
{
    auto it = m_typeRefs.find(typeRef.name());

    if (it == m_typeRefs.end()) {
        return false;
    }

    m_typeRefs.erase(it);
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

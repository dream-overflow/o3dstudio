/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/entity.h"

using namespace o3d::studio::common;


Entity::Entity(const String &name, Entity *parent) :
    m_name(name),
    m_parent(parent),
    m_capacities()
{
    setDirty();
}

Entity::~Entity()
{

}

void Entity::setRef(const ObjectRef &ref)
{
    m_ref = ref;
    setDirty();
}

Entity *Entity::parent()
{
    return m_parent;
}

const Entity *Entity::parent() const
{
    return m_parent;
}

void Entity::setName(const String &name)
{
    m_name = name;
    setDirty();
}

const o3d::String &Entity::name() const
{
    return m_name;
}

const ObjectRef &Entity::ref() const
{
    return m_ref;
}

void Entity::setTypeRef(const TypeRef &typeRef)
{
    m_typeRef = typeRef;
}

const TypeRef& Entity::typeRef() const
{
    return m_typeRef;
}

o3d::Bool Entity::exists() const
{
    return m_ref.light().isValid();
}

o3d::Bool Entity::hasChanges() const
{
    return isDirty();
}

o3d::Bool Entity::serializeContent(QDataStream &stream) const
{
    stream << m_name
           << m_parent->ref().uuid()
           << m_parent->ref().strong().typeName();

    return True;
}

o3d::Bool Entity::deserializeContent(QDataStream &stream)
{
    Uuid uuid;
    String typeName;

    stream >> m_name
           >> uuid
           >> typeName;

    return True;
}

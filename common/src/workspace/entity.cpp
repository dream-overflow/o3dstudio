/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/entity.h"

using namespace o3d::studio::common;


Entity::Entity(const QString &name, Entity *parent) :
    m_name(name),
    m_parent(parent),
    m_capacities()
{
    m_capacities.resize(64);
}

Entity::~Entity()
{

}

void Entity::setRef(const ObjectRef &ref)
{
    m_ref = ref;
}

Entity *Entity::parent()
{
    return m_parent;
}

const Entity *Entity::parent() const
{
    return m_parent;
}

const QString &Entity::name() const
{
    return m_name;
}

const ObjectRef &Entity::ref() const
{
    return m_ref;
}

const TypeRef& Entity::typeRef() const
{
    return m_typeRef;
}

bool Entity::exists() const
{
    return m_ref.light().isValid();
}

bool Entity::hasChanges()
{
    return isDirty();
}

bool Entity::serializeContent(QDataStream &stream) const
{
    stream << m_name
           << m_parent->ref().uuid()
           << m_parent->ref().strong().type();

    return true;
}

bool Entity::deserializeContent(QDataStream &stream)
{
    QUuid uuid;
    QString typeName;

    stream >> m_name
           >> uuid
           >> typeName;

    return true;
}

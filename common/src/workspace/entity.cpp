/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/entity.h"

using namespace o3d::studio::common;


Entity::Entity(Entity *parent) :
    m_parent(parent)
{

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

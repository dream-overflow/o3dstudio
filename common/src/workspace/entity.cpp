/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/entity.h"

#include <o3d/core/file.h>

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

void Entity::setParent(Entity *entity)
{
    m_parent = entity;
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

o3d::Bool Entity::isParentHub() const
{
    return m_parent && m_parent->ref().light().baseTypeOf(TypeRef::hub());
}

void Entity::setActiveSelection(o3d::Bool activity)
{
    m_capacities.setBit(STATE_ACTIVE_SELECTION, activity);
}

o3d::Bool Entity::isActiveSelection() const
{
    return m_capacities.getBit(STATE_ACTIVE_SELECTION);
}

void Entity::setSelected(o3d::Bool selected)
{
    m_capacities.setBit(STATE_SELECTED, selected);
}

o3d::Bool Entity::isSelected() const
{
    return m_capacities.getBit(STATE_SELECTED);
}

void Entity::setActivity(o3d::Bool activity)
{
    m_capacities.setBit(STATE_ACTIVITY, activity);
}

o3d::Bool Entity::isActive() const
{
    return m_capacities.getBit(STATE_ACTIVITY);
}

void Entity::setVisibility(o3d::Bool visibility)
{
    m_capacities.setBit(STATE_VISIBILITY, visibility);
}

o3d::Bool Entity::isVisible() const
{
    return m_capacities.getBit(STATE_VISIBILITY);
}

o3d::Bool Entity::serializeContent(OutStream &stream) const
{
    stream << m_name
           << m_parent->ref().uuid()
           << m_parent->ref().strong().typeName()
           << m_capacities.getBit(STATE_ACTIVITY)
           << m_capacities.getBit(STATE_VISIBILITY);

    return True;
}

o3d::Bool Entity::deserializeContent(InStream &stream)
{
    Uuid uuid;
    String typeName;

    Bool activity, visibility;

    stream >> m_name
           >> uuid
           >> typeName
           >> activity
           >> visibility;

    m_capacities.setBit(STATE_ACTIVITY, activity);
    m_capacities.setBit(STATE_VISIBILITY, visibility);

    return True;
}

o3d::Bool Entity::writeToFile(o3d::OutStream &os) const
{
    return serializeContent(os);
}

o3d::Bool Entity::readFromFile(o3d::InStream &is)
{
    return deserializeContent(is);
}

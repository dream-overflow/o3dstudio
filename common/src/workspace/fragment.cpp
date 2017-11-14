/**
 * @brief Common fragment object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


Fragment::Fragment(const QString &name, Entity *parent) :
    Entity(name, parent)
{
    m_ref = ObjectRef(TypeRef::fragment());
}

Fragment::~Fragment()
{
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        delete hub;
    }
}

void Fragment::setProject(Project *project)
{
    m_parent = project;
}

Project *Fragment::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Fragment::project() const
{
    return static_cast<Project*>(m_parent);
}

void Fragment::create()
{

}

bool Fragment::load()
{
    return true;
}

bool Fragment::save()
{
    return true;
}

bool Fragment::exists() const
{
    return Entity::exists();
}

bool Fragment::serializeContent(QDataStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return false;
    }

    return true;
}

bool Fragment::deserializeContent(QDataStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return false;
    }

    return false;
}
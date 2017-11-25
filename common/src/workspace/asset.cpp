/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


Asset::Asset(const String &name, Entity *parent) :
    Entity(name, parent)
{
    m_typeRef = TypeRef::asset();
    m_ref = ObjectRef(TypeRef::asset());
}

Asset::~Asset()
{
//    Hub *hub = nullptr;
//    for (auto it ...
//        delete hub;
//    }
}

void Asset::setProject(Project *project)
{
    m_parent = project;
}

Project *Asset::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Asset::project() const
{
    return static_cast<Project*>(m_parent);
}

o3d::Int32 Asset::childIndexOf(Entity *entity) const
{
    // @todo
    return -1;
}

void Asset::create()
{
    setDirty();
}

o3d::Bool Asset::load()
{
    return True;
}

o3d::Bool Asset::save()
{
    return True;
}

o3d::Bool Asset::exists() const
{
    return Entity::exists();
}

o3d::Bool Asset::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool Asset::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    return False;
}

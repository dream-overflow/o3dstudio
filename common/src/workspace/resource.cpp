/**
 * @brief Common resource object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include "o3d/studio/common/workspace/resource.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"

using namespace o3d::studio::common;


Resource::Resource(const String &name, Entity *parent) :
    Entity(name, parent),
    m_project(nullptr)
{
    m_typeRef = TypeRef::resource();
    m_ref = ObjectRef(TypeRef::resource());

    if (parent) {
        // auto set project from parent
        m_project = parent->project();
    }
}

Resource::~Resource()
{

}

void Resource::setProject(Project *project)
{
    m_project = project;
}

Project *Resource::project()
{
    return m_project;
}

const Project *Resource::project() const
{
    return m_project;
}

void Resource::create()
{
    setDirty();

    project()->addEntity(this);

    // @todo and storage manager...
    // signal throught project->workspace
    // project()->workspace()->onProjectResourceAdded(ref().light());
}

void Resource::destroy()
{
    // signal throught project->workspace
    // project()->workspace()->onProjectResourceRemoved(ref().light());
}

o3d::Bool Resource::load()
{
    return True;
}

o3d::Bool Resource::save()
{
    return True;
}

o3d::Bool Resource::exists() const
{
    return Entity::exists();
}

o3d::Bool Resource::deletable() const
{
    return True;
}

o3d::Int32 Resource::childIndexOf(Entity *entity) const
{
    if (!entity || entity->ref().light().projectId() != project()->ref().light().id()) {
        return -1;
    }

    UInt64 id = entity->ref().light().id();
    Int32 n = 0;

    return -1;
}

//void Resource::removeFromScene(MasterScene *masterScene)
//{

//}

o3d::Bool Resource::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool Resource::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    // setup now
    create();

    // @todo

    return True;
}

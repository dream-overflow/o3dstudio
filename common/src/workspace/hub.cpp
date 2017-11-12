/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


Hub::Hub(const QString &name, Project *project) :
    Entity(name, project)
{
    m_ref = ObjectRef::buildRef(project, TypeRef::hub());
}

Hub::~Hub()
{
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        delete hub;
    }
}

//void Hub::setFragment(Fragment *fragment)
//{
//    m_parent = fragment;
//}

//Fragment *Hub::fragment()
//{
//    return static_cast<Fragment*>(m_parent);
//}

//const Fragment *Hub::fragment() const
//{
//    return static_cast<Fragment*>(m_parent);
//}

void Hub::setProject(Project *project)
{
    m_parent = project;
}

Project *Hub::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Hub::project() const
{
    return static_cast<Project*>(m_parent);
}

void Hub::create()
{

}
bool Hub::load()
{
    return true;
}

bool Hub::save()
{
    return true;
}

bool Hub::exists() const
{
    return false;
}

bool Hub::hasChanges()
{
    return false;
}

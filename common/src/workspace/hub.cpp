/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


Hub::Hub(const QString &name, Project *project) :
    m_project(project),
    m_name(name),
    m_ref()
{
    m_ref = ObjectRef::buildRef(project, TypeRef());  // @todo hub ref type
}

Hub::~Hub()
{

}

void Hub::setProject(Project *project)
{
    m_project = project;
}

Project *Hub::project()
{
    return m_project;
}

const Project *Hub::project() const
{
    return m_project;
}

void Hub::create()
{

}

const ObjectRef &Hub::ref() const
{
    return m_ref;
}

void Hub::setRef(const ObjectRef &ref)
{
    m_ref = ref;
}

const QString &Hub::name() const
{
    return m_name;
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

/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "common/workspace/project.h"
#include "common/workspace/workspace.h"

using namespace o3d::studio::common;

Project::Project(Workspace *workspace, const QString &name) :
    m_workspace(workspace),
    m_name(name)
{

}

Project::~Project()
{

}

const QString& Project::getName() const
{
    return m_name;
}

const QString& Project::getFilename() const
{
    return m_filename;
}

bool Project::setLocation(const QString &path)
{
    // @todo
    return true;
}

bool Project::load()
{
    return true;
}

bool Project::save()
{
    return true;
}

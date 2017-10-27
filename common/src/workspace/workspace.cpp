/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "common/workspace/workspace.h"
#include "common/workspace/project.h"

using namespace o3d::studio::common;

Workspace::Workspace(const QString &name) :
    m_name(name)
{

}

Workspace::~Workspace()
{

}

const QString& Workspace::getName() const
{
    return m_name;
}

const QString& Workspace::getFilename() const
{
    return m_filename;
}

QStringList Workspace::getProjectsList() const
{
    return m_foundProjects;
}

bool Workspace::hasChanged() const
{
    // @todo

    return false;
}

bool Workspace::selectWorkspace(const QString &name)
{
    auto it = m_loadedProjects.find(name);
    if (it != m_loadedProjects.end()) {
        m_selectedProject = it.value();
        return true;
    }

    return false;
}

bool Workspace::save()
{
    return true;
}

bool Workspace::load()
{
    return true;
}

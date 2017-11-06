/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;

Workspace::Workspace(const QString &name) :
    m_name(name)
{

}

Workspace::~Workspace()
{
    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (project->hasChanges()) {
            project->save();
        }

        delete project;
    }
}

const QString& Workspace::name() const
{
    return m_name;
}

const QString& Workspace::filename() const
{
    return m_filename;
}

Project *Workspace::project(const QUuid &uuid)
{
    auto it = m_loadedProjects.find(uuid);
    if (it != m_loadedProjects.end()) {
        return it.value();
    }

    return nullptr;
}

const Project *Workspace::project(const QUuid &uuid) const
{
    auto cit = m_loadedProjects.find(uuid);
    if (cit != m_loadedProjects.cend()) {
        return cit.value();
    }

    return nullptr;
}

QStringList Workspace::projectsList() const
{
    return m_foundProjects;
}

bool Workspace::addProject(Project *project)
{
    if (!project) {
        return false;
    }

    Q_ASSERT(m_loadedProjects.find(project->uuid()) == m_loadedProjects.end());
    m_loadedProjects.insert(project->uuid(), project);

    return true;
}

bool Workspace::closeProject(const QUuid& uuid)
{
    auto it = m_loadedProjects.find(uuid);
    if (it != m_loadedProjects.end()) {
        Project *project = it.value();

        m_loadedProjects.erase(it);

        if (project == m_activeProject) {
            m_activeProject = nullptr;
            // @todo signal change active project
        }

        project->save();
        delete project;

        return true;
    }

    return false;
}

bool Workspace::hasProject(const QUuid& uuid)
{
    auto cit = m_loadedProjects.find(uuid);
    return cit != m_loadedProjects.cend();
}

bool Workspace::hasChanges() const
{
    // at least one project has changes to be saved
    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (project->hasChanges()) {
            return true;
        }
    }

    return false;
}

bool Workspace::selectProject(const QUuid &uuid)
{
    auto it = m_loadedProjects.find(uuid);
    if (it != m_loadedProjects.end()) {
        m_activeProject = it.value();
        return true;
    }

    return false;
}

bool Workspace::save()
{
    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (!project->save()) {
            // @todo what ?
        }
    }

    return true;
}

bool Workspace::load()
{
    return true;
}

WorkspaceException::WorkspaceException(const QString &message) :
    BaseException(message)
{

}

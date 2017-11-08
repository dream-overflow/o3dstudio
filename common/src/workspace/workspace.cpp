/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

using namespace o3d::studio::common;

Workspace::Workspace(const QString &name, QObject *parent) :
    QObject(),
    m_name(name)
{
    // selection manager
    connect(&Application::instance()->selection(), SIGNAL(selectionChanged()), SLOT(onSelectionChanged()));
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

const QUuid &Workspace::uuid() const
{
    return m_uuid;
}

const QString& Workspace::name() const
{
    return m_name;
}

const QString& Workspace::filename() const
{
    return m_filename;
}

void Workspace::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
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

QList<Project*> Workspace::loadedProjectList() const
{
    return m_loadedProjects.values();
}

bool Workspace::addProject(Project *project)
{
    if (!project) {
        return false;
    }

    Q_ASSERT(m_loadedProjects.find(project->uuid()) == m_loadedProjects.end());
    m_loadedProjects.insert(project->uuid(), project);

    emit onProjectAdded(project->uuid());

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

bool Workspace::hasProject(QString location) const
{
    location = QDir::cleanPath(location);

    if (location.endsWith(QDir::separator())) {
        location.chop(1);
    }

    auto cit = m_loadedProjects.cbegin();
    while (cit != m_loadedProjects.cend()) {
        if (cit.value()->path().absolutePath() == location) {
            return true;
        }

        ++cit;
    }

    return false;
}

bool Workspace::hasProject(const QUuid& uuid) const
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

        emit onProjectActivated(uuid);

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

void Workspace::onSelectionChanged()
{
    const QSet<SelectionItem *> previousSelection = Application::instance()->selection().filterPrevious(
                                              common::SelectionItem::SELECTION_PROJECT);

    const QSet<SelectionItem *> currentSelection = Application::instance()->selection().filterCurrent(
                                              common::SelectionItem::SELECTION_PROJECT);

    bool changeProject = false;

    if (!previousSelection.isEmpty()) {
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, previousSelection) {
            if (selectionItem->uuid() == m_activeProject->uuid()) {
                m_activeProject = nullptr;
                changeProject = true;
                break;
            }
        }
    }

    if (!currentSelection.isEmpty()) {
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, previousSelection) {
            Project *lproject = project(selectionItem->uuid());
            if (lproject) {
                m_activeProject = lproject;
                changeProject = true;
                break;
            }
        }
    }

    if (changeProject) {
        if (m_activeProject) {
            emit onProjectActivated(m_activeProject->uuid());
        } else {
            emit onProjectActivated(QUuid());
        }
    }
}

WorkspaceException::WorkspaceException(const QString &message) :
    BaseException(message)
{

}

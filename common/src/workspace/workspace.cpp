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
#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::common;

Workspace::Workspace(const QString &name, QObject *parent) :
    QObject(parent),
    m_nextId(1),
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

quint32 Workspace::generateProjectId()
{
    quint64 nextId = m_nextId++;
    return nextId;
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

Project *Workspace::project(const LightRef &ref)
{
    // id can comes from a project reference or a entity of this project
    quint64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto it = m_loadedProjects.find(id);
    if (it != m_loadedProjects.end()) {
        return it.value();
    }

    return nullptr;
}

const Project *Workspace::project(const LightRef &ref) const
{
    // id can comes from a project reference or a entity of this project
    quint64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto cit = m_loadedProjects.find(id);
    if (cit != m_loadedProjects.cend()) {
        return cit.value();
    }

    return nullptr;
}

Project *Workspace::activeProject()
{
    return m_activeProject;
}

const Project *Workspace::activeProject() const
{
    return m_activeProject;
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

    auto it = m_loadedProjects.find(project->ref().light().id());
    Q_ASSERT(it == m_loadedProjects.end());
    if (it != m_loadedProjects.end()) {
        return false;
    }

    m_loadedProjects.insert(project->ref().light().id(), project);

    // signal
    emit onProjectAdded(project->ref().light());

    return true;
}

bool Workspace::closeProject(const LightRef &ref)
{
    auto it = m_loadedProjects.find(ref.id());
    if (it != m_loadedProjects.end()) {
        Project *project = it.value();

        m_loadedProjects.erase(it);

        if (project == m_activeProject) {
            m_activeProject = nullptr;
        }

        project->save();

        emit onProjectRemoved(ref);

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

bool Workspace::hasProject(const LightRef& ref) const
{
    // id can comes from a project reference or a entity of this project
    qint64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto cit = m_loadedProjects.find(id);
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

bool Workspace::setActiveProject(const LightRef &ref)
{
    // id can comes from a project reference or a entity of this project
    quint64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto it = m_loadedProjects.find(id);
    if (it != m_loadedProjects.end()) {
        m_activeProject = it.value();

        emit onProjectActivated(ref);

        return true;
    }

    return false;
}

bool Workspace::save()
{
    messenger().info(tr("Saving current workspace..."));

    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (!project->save()) {
            // @todo what ?
        }
    }

    messenger().info(tr("Current workspace saved !"));

    return true;
}

bool Workspace::load()
{
    return true;
}

Hub *Workspace::hub(const LightRef &ref)
{
    if (ref.baseTypeOf(TypeRef::hub())) {
        auto it = m_loadedProjects.find(ref.projectId());
        if (it != m_loadedProjects.end()) {
            Project *project = it.value();
            return project->hub(ref.id());
        }
    }

    return nullptr;
}

const Hub *Workspace::hub(const LightRef &ref) const
{
    if (ref.baseTypeOf(TypeRef::hub())) {
        auto cit = m_loadedProjects.constFind(ref.projectId());
        if (cit != m_loadedProjects.cend()) {
            const Project *project = cit.value();
            return project->hub(ref.id());
        }
    }

    return nullptr;
}

Hub *Workspace::findHub(const LightRef &ref)
{
    if (ref.baseTypeOf(TypeRef::hub())) {
        auto it = m_loadedProjects.find(ref.projectId());
        if (it != m_loadedProjects.cend()) {
            Project *project = it.value();
            return project->findHub(ref.id());
        }
    }

    return nullptr;
}

const Hub *Workspace::findHub(const LightRef &ref) const
{
    if (ref.baseTypeOf(TypeRef::hub())) {
        auto cit = m_loadedProjects.constFind(ref.projectId());
        if (cit != m_loadedProjects.cend()) {
            const Project *project = cit.value();
            return project->findHub(ref.id());
        }
    }

    return nullptr;
}

Fragment *Workspace::fragment(const LightRef &ref)
{
    if (ref.baseTypeOf(TypeRef::fragment())) {
        auto it = m_loadedProjects.find(ref.projectId());
        if (it != m_loadedProjects.end()) {
            Project *project = it.value();
            return project->fragment(ref.id());
        }
    }

    return nullptr;
}

const Fragment *Workspace::fragment(const LightRef &ref) const
{
    if (ref.baseTypeOf(TypeRef::fragment())) {
        auto cit = m_loadedProjects.constFind(ref.projectId());
        if (cit != m_loadedProjects.cend()) {
            const Project *project = cit.value();
            return project->fragment(ref.id());
        }
    }

    return nullptr;
}

Asset *Workspace::asset(const LightRef &ref)
{
    if (ref.baseTypeOf(TypeRef::asset())) {
        auto it = m_loadedProjects.find(ref.projectId());
        if (it != m_loadedProjects.end()) {
            Project *project = it.value();
            return project->asset(ref.id());
        }
    }

    return nullptr;
}

const Asset *Workspace::asset(const LightRef &ref) const
{
    if (ref.baseTypeOf(TypeRef::asset())) {
        auto cit = m_loadedProjects.constFind(ref.projectId());
        if (cit != m_loadedProjects.cend()) {
            const Project *project = cit.value();
            return project->asset(ref.id());
        }
    }

    return nullptr;
}

void Workspace::onSelectionChanged()
{
    // done by the UI depending of the context, the need, and the selection
    return;

    const QSet<SelectionItem *> previousSelection =
            Application::instance()->selection().filterPreviousByBaseType(TypeRef::project());
    const QSet<SelectionItem *> currentSelection =
            Application::instance()->selection().filterCurrentByBaseType(TypeRef::project());

    bool changeProject = false;

    if (!previousSelection.isEmpty()) {
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, previousSelection) {
            if (selectionItem->ref() == m_activeProject->ref().light()) {
                m_activeProject = nullptr;
                changeProject = true;
                break;
            }
        }
    }

    if (!currentSelection.isEmpty()) {
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, previousSelection) {
            Project *lproject = project(selectionItem->ref());
            if (lproject) {
                m_activeProject = lproject;
                changeProject = true;
                break;
            }
        }
    }

    if (changeProject) {
        if (m_activeProject) {
            emit onProjectActivated(m_activeProject->ref().light());
        } else {
            emit onProjectActivated(LightRef());
        }
    }
}

Messenger &Workspace::messenger()
{
    return Application::instance()->messenger();
}

WorkspaceException::WorkspaceException(const QString &message) :
    BaseException(message)
{

}

/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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

#include <o3d/core/diskdir.h>

using namespace o3d::studio::common;

Workspace::Workspace(const String &name, BaseObject *parent) :
    BaseObject(parent),
    m_nextId(1),
    m_name(name)
{
    // selection manager
    Application::instance()->selection().selectionChanged.connect(this, &Workspace::onSelectionChanged);
}

Workspace::~Workspace()
{
    Project *project = nullptr;
    for (auto it = m_loadedProjects.begin(); it != m_loadedProjects.end(); ++it) {
        project = it->second;

        if (project->hasChanges()) {
            project->save();
        }

        delete project;
    }
}

o3d::UInt32 Workspace::generateProjectId()
{
    UInt32 nextId = m_nextId++;
    return nextId;
}

const o3d::Uuid &Workspace::uuid() const
{
    return m_uuid;
}

const o3d::String& Workspace::name() const
{
    return m_name;
}

const o3d::String& Workspace::filename() const
{
    return m_filename;
}

void Workspace::setUuid(const Uuid &uuid)
{
    m_uuid = uuid;
}

Project *Workspace::project(const LightRef &ref)
{
    // id can comes from a project reference or a entity of this project
    UInt64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto it = m_loadedProjects.find(id);
    if (it != m_loadedProjects.end()) {
        return it->second;
    }

    return nullptr;
}

const Project *Workspace::project(const LightRef &ref) const
{
    // id can comes from a project reference or a entity of this project
    UInt64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto cit = m_loadedProjects.find(id);
    if (cit != m_loadedProjects.cend()) {
        return cit->second;
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

o3d::T_StringList Workspace::projectsList() const
{
    return m_foundProjects;
}

std::list<Project*> Workspace::loadedProjectList() const
{
    std::list<Project*> projects;

    for (auto it = m_loadedProjects.begin(); it != m_loadedProjects.end(); ++it) {
        projects.push_back(it->second);
    }

    return projects;
}

o3d::Bool Workspace::addProject(Project *project)
{
    if (!project) {
        return False;
    }

    auto it = m_loadedProjects.find(project->ref().light().id());
    O3D_ASSERT(it == m_loadedProjects.end());
    if (it != m_loadedProjects.end()) {
        return False;
    }

    m_loadedProjects[project->ref().light().id()] = project;

    // signal
    onProjectAdded(project->ref().light());

    return True;
}

o3d::Bool Workspace::closeProject(const LightRef &ref)
{
    auto it = m_loadedProjects.find(ref.id());
    if (it != m_loadedProjects.end()) {
        Project *project = it->second;

        m_loadedProjects.erase(it);

        if (project == m_activeProject) {
            m_activeProject = nullptr;
        }

        project->save();

        onProjectRemoved(ref);

        delete project;
        return True;
    }

    return False;
}

o3d::Bool Workspace::hasProject(String location) const
{
    DiskDir path(location);

    auto cit = m_loadedProjects.cbegin();
    while (cit != m_loadedProjects.cend()) {
        if (cit->second->path() == location) {
            return True;
        }

        ++cit;
    }

    return False;
}

o3d::Bool Workspace::hasProject(const LightRef& ref) const
{
    // id can comes from a project reference or a entity of this project
    UInt64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto cit = m_loadedProjects.find(id);
    return cit != m_loadedProjects.cend();
}

o3d::Bool Workspace::hasChanges() const
{
    // at least one project has changes to be saved
    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (project->hasChanges()) {
            return True;
        }
    }

    return False;
}

o3d::Bool Workspace::setActiveProject(const LightRef &ref)
{
    // id can comes from a project reference or a entity of this project
    UInt64 id = ref.baseTypeOf(TypeRef::project()) ? ref.id() : ref.projectId();

    auto it = m_loadedProjects.find(id);
    if (it != m_loadedProjects.end()) {
        m_activeProject = it.value();

        onProjectActivated(ref);

        return True;
    }

    return False;
}

o3d::Bool Workspace::save()
{
    messenger().info(fromQString(tr("Saving current workspace...")));

    Project *project = nullptr;
    foreach (project, m_loadedProjects) {
        if (!project->save()) {
            // @todo what ?
        }
    }

    messenger().info(fromQString(tr("Current workspace saved !")));

    return True;
}

o3d::Bool Workspace::load()
{
    return True;
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

    const std::set<SelectionItem *> previousSelection =
            Application::instance()->selection().filterPreviousByBaseType(TypeRef::project());
    const std::set<SelectionItem *> currentSelection =
            Application::instance()->selection().filterCurrentByBaseType(TypeRef::project());

    Bool changeProject = False;

    if (!previousSelection.isEmpty()) {
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, previousSelection) {
            if (selectionItem->ref() == m_activeProject->ref().light()) {
                m_activeProject = nullptr;
                changeProject = True;
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
                changeProject = True;
                break;
            }
        }
    }

    if (changeProject) {
        if (m_activeProject) {
            onProjectActivated(m_activeProject->ref().light());
        } else {
            onProjectActivated(LightRef());
        }
    }
}

Messenger &Workspace::messenger()
{
    return Application::instance()->messenger();
}

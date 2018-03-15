/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/storage/store.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/roothub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/projectinfo.h"
#include "o3d/studio/common/workspace/projectfile.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include <o3d/core/filemanager.h>

using namespace o3d::studio::common;

Project::Project(const String &name, Workspace *workspace) :
    Entity(name, nullptr),
    m_workspace(workspace),
    m_filename(""),
    m_path(FileManager::instance()->getWorkingDirectory()),
    m_projectFile(nullptr),
    m_nextId(1),
    m_info(nullptr),
    m_masterScene(nullptr),
    m_rootHub(nullptr)
{
    m_typeRef = TypeRef::project();

    m_ref = ObjectRef::buildRef(workspace);
    m_masterScene = new MasterScene(this);
    m_projectFile = new ProjectFile(this, ProjectFile::PROJECT_VERSION_1_0_0);
    m_info = new ProjectInfo();

    // create the virtual root hub (created at project add)
    m_rootHub = new RootHub("RootHub", this);
    m_rootHub->setTypeRef(Application::instance()->types().typeRef("o3s::common::hub::root"));

    // valid but invalid if project id is changed before usage and after ctor
    m_rootHub->setRef(ObjectRef::buildRef(this, m_rootHub->typeRef()));    
}

Project::~Project()
{
    // force deletion of deferred entities
    Entity *entity = nullptr;
    for (auto it = m_deferredDelete.begin(); it != m_deferredDelete.end(); ++it) {
        entity = *it;
        deletePtr(entity);
    }

    // then, the fragments
    Fragment *fragment = nullptr;
    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {
        fragment = it->second;

        // @todo could need a scene cleaning step
        deletePtr(fragment);
    }

    // @todo need context setup and
    // hubs (previously cleaned on fragment just before)
    std::list<Hub*> hubs = m_rootHub->hubs(True);
    for (auto rit = hubs.rbegin(); rit != hubs.rend(); ++rit) {
        (*rit)->removeFromScene(m_masterScene);
    }
    delete m_rootHub;

    // now we can safely delete the master scene with the o3d scene
    delete m_masterScene;
    delete m_projectFile;
    delete m_info;
}

Project *Project::project()
{
    return this;
}

const Project *Project::project() const
{
    return this;
}

void Project::setWorkspace(Workspace *workspace)
{
    m_workspace = workspace;
}

Workspace *Project::workspace()
{
    return m_workspace;
}

const Workspace *Project::workspace() const
{
    return m_workspace;
}

o3d::UInt64 Project::generateEntityId()
{
    UInt64 nextId = m_nextId++;
    return nextId;
}

o3d::Int32 Project::childIndexOf(Entity *entity) const
{
    if (!entity || entity->ref().light().projectId() != project()->ref().light().id()) {
        return -1;
    }

//    UInt64 id = entity->ref().light().id();
//    Int32 n = 0;

    if (entity->ref().light().baseTypeOf(TypeRef::hub()) && entity->name() == "RootHub") {
        return 0;
    }

    return -1;
}

o3d::Bool Project::hasChanges() const
{
    if (isDirty()) {
        return True;
    }

    Entity *entity = nullptr;
    for (auto it = m_entitiesById.begin(); it != m_entitiesById.end(); ++it) {
        entity = it->second;
        if (entity->hasChanges()) {
            return True;
        }
    }

    return False;
}

void Project::create()
{
    if (exists()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Project already exists"))));
    };

    // project structure
    Application::instance()->store().initProject(this);

    // and project file
    m_projectFile->create();

    // never saved
    setDirty();
}

void Project::destroy()
{
    // nothing to do
}

void Project::update()
{
    // nothing to do
}

o3d::Bool Project::deletable() const
{
    return m_deferredDelete.empty() && m_rootHub->deletable();
}

o3d::String Project::filename() const
{
    return m_path.makeFullFileName("project.o3dstudio");
}

const o3d::LocalDir &Project::path() const
{
    return m_path;
}

const ProjectInfo &Project::info() const
{
    return *m_info;
}

ProjectInfo &Project::info()
{
    // potentialy changed
    setDirty();

    return *m_info;
}

o3d::Bool Project::setLocation(const LocalDir &path)
{
    if (!path.exists()) {
        return False;
    }

    m_path = path.getFullPathName() + '/' + m_name;

    return True;
}

o3d::Bool Project::load()
{
    if (!exists()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Project doesn't exists"))));
    };

    // project structure
    Application::instance()->store().loadProject(this);

    // project file
    m_projectFile->load();

    // store indexe
    // @todo

    return True;
}

o3d::Bool Project::save()
{
    if (!exists()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Project doesn't exists"))));
    };

    // project structure
    Application::instance()->store().saveProject(this);

    // and project file
    m_projectFile->save();

    // store indexe
    // @todo

    setClean();
    return True;
}

o3d::Bool Project::exists() const
{
    return m_path.exists() && m_path.check("project.o3dstudio") == BaseDir::SUCCESS;
}

MasterScene *Project::masterScene()
{
    return m_masterScene;
}

const MasterScene *Project::masterScene() const
{
    return m_masterScene;
}

void Project::setupMasterScene()
{
    if (m_masterScene) {
        m_masterScene->initialize();
    }
}

Entity *Project::lookup(const LightRef &ref)
{
    if (ref.projectId() == m_ref.light().id()) {
        auto it = m_entitiesById.find(ref.id());
        if (it != m_entitiesById.end()) {
            return it->second;
        }
    }

    return nullptr;
}

const Entity *Project::lookup(const LightRef &ref) const
{
    if (ref.projectId() == m_ref.light().id()) {
        auto cit = m_entitiesById.find(ref.id());
        if (cit != m_entitiesById.cend()) {
            return cit->second;
        }
    }

    return nullptr;
}

Entity *Project::lookup(const Uuid &uuid)
{
    auto it = m_entitiesByUuid.find(uuid);
    if (it != m_entitiesByUuid.end()) {
        return it->second;
    }

    return nullptr;
}

const Entity *Project::lookup(const Uuid &uuid) const
{
    auto cit = m_entitiesByUuid.find(uuid);
    if (cit != m_entitiesByUuid.cend()) {
        return cit->second;
    }

    return nullptr;
}

void Project::addFragment(Fragment *fragment)
{
    // not created for this project
    if (fragment->ref().light().projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a fragment that is created for another project"))));
    }

    // already exists
    if (m_fragments.find(fragment->ref().light().id()) != m_fragments.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added fragment, or with a similar id"))));
    }

    m_fragments[fragment->ref().light().id()] = fragment;
    addEntity(fragment);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectFragmentAdded(fragment->ref().light());
}

void Project::removeFragment(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove a reference for another project"))));
    }

    auto it = m_fragments.find(_ref.id());
    if (it == m_fragments.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Fragment *fragment = it->second;
    m_fragments.erase(it);

    // structure change
    setDirty();

    // deferred deletion
    deleteEntity(fragment);

    // signal throught workspace
    workspace()->onProjectFragmentRemoved(_ref);
}

void Project::removeFragment(UInt64 id)
{
    auto it = m_fragments.find(id);
    if (it == m_fragments.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Fragment *fragment = it->second;
    LightRef _ref = fragment->ref().light();
    m_fragments.erase(it);

    // structure change
    setDirty();

    // deferred deletion
    deleteEntity(fragment);

    // signal throught workspace
    workspace()->onProjectFragmentRemoved(_ref);
}

void Project::removeFragment(Fragment *fragment)
{
    if (!fragment) {
        return;
    }

    UInt64 fragmentId = fragment->ref().light().id();
    auto it = m_fragments.find(fragmentId);

    if (it != m_fragments.end()) {
        LightRef _ref = fragment->ref().light();
        m_fragments.erase(it);

        // structure change
        setDirty();

        // deferred deletion
        deleteEntity(fragment);

        // signal throught workspace
        workspace()->onProjectFragmentRemoved(_ref);

        return;
    }
}

Fragment *Project::fragment(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto it = m_fragments.find(_ref.id());
    if (it != m_fragments.end()) {
        return it->second;
    }

    return nullptr;
}

const Fragment *Project::fragment(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_fragments.find(_ref.id());
    if (cit != m_fragments.cend()) {
        return cit->second;
    }

    return nullptr;
}

Fragment *Project::fragment(UInt64 id)
{
    auto it = m_fragments.find(id);
    if (it != m_fragments.end()) {
        return it->second;
    }

    return nullptr;
}

const Fragment *Project::fragment(UInt64 id) const
{
    auto cit = m_fragments.find(id);
    if (cit != m_fragments.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<Fragment *> Project::searchFragment(const String &name)
{
    std::list<Fragment*> results;

    Fragment *fragment;
    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {
        fragment = it->second;

        if (fragment->name() == name) {
            results.push_back(fragment);
        }
    }

    return results;
}

std::list<const Fragment *> Project::searchFragment(const String &name) const
{
    std::list<const Fragment*> results;

    const Fragment *fragment;
    for (auto cit = m_fragments.cbegin(); cit != m_fragments.cend(); ++cit) {
        fragment = cit->second;

        if (fragment->name() == name) {
            results.push_back(fragment);
        }
    }

    return results;
}

std::list<Fragment *> Project::fragments()
{
    // first level
    std::list<Fragment*> results;
    Fragment *fragment = nullptr;

    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {
        fragment = it->second;
        results.push_back(fragment);
    }

    return results;
}

std::list<const Fragment *> Project::fragments() const
{
    // first level
    std::list<const Fragment*> results;
    const Fragment *fragment = nullptr;

    for (auto cit = m_fragments.cbegin(); cit != m_fragments.cend(); ++cit) {
        fragment = cit->second;
        results.push_back(fragment);
    }

    return results;
}

void Project::addPickable(UInt32 id, Hub *hub)
{
    if (!hub) {
        return;
    }

    m_hubsByPickableId[id] = hub;
}

void Project::removePickable(UInt32 id)
{
    auto it = m_hubsByPickableId.find(id);
    if (it != m_hubsByPickableId.end()) {
        m_hubsByPickableId.erase(it);
    }
}

Hub* Project::lookupPickable(o3d::UInt32 id)
{
    auto it = m_hubsByPickableId.find(id);
    if (it != m_hubsByPickableId.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Project::lookupPickable(o3d::UInt32 id) const
{
    auto cit = m_hubsByPickableId.find(id);
    if (cit != m_hubsByPickableId.cend()) {
        return cit->second;
    }

    return nullptr;
}

void Project::addEntity(Entity *entity)
{
    if (!entity) {
        return;
    }

    UInt64 id = entity->ref().light().id();

    m_entitiesById[id] = entity;
    m_entitiesByUuid[entity->ref().uuid()] = entity;
}

void Project::removeEntity(Entity *entity)
{
    if (!entity) {
        return;
    }

    auto it2 = m_entitiesById.find(entity->ref().light().id());
    if (it2 != m_entitiesById.end()) {
        m_entitiesById.erase(it2);
    }

    auto it3 = m_entitiesByUuid.find(entity->ref().uuid());
    if (it3 != m_entitiesByUuid.end()) {
        m_entitiesByUuid.erase(it3);
    }

    // reset the reference
    entity->setRef(ObjectRef((entity->typeRef())));
}

void Project::deleteEntity(Entity *entity)
{
    if (!entity) {
        return;
    }

    // att to deferred delete
    m_deferredDelete.push_back(entity);
}

void Project::purgeEntities()
{
    Entity *entity = nullptr;
    auto it = m_deferredDelete.begin();
    while (it != m_deferredDelete.end()) {
        entity = *it;

        if (entity->deletable()) {
           delete(entity);
            it = m_deferredDelete.erase(it);
        } else {
            ++it;
        }
    }
}

Hub *Project::rootHub()
{
    return m_rootHub;
}

Hub *Project::rootHub() const
{
    return m_rootHub;
}

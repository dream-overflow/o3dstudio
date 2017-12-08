/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/storage/store.h"

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/asset.h"
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
    m_masterScene(nullptr)
{
    m_typeRef = TypeRef::project();

    m_ref = ObjectRef::buildRef(workspace);
    m_masterScene = new MasterScene(this);
    m_projectFile = new ProjectFile(this, ProjectFile::PROJECT_VERSION_1_0_0);
    m_info = new ProjectInfo();
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

    // hubs
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        // first clean from scene
        hub->removeFromScene(m_masterScene);
    }

    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        // next we can finally delete
        deletePtr(hub);
    }
    m_hubsOrder.clear();

    // and finally assets
    Asset *asset = nullptr;
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        asset = it->second;
        deletePtr(asset);
    }

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

    UInt64 id = entity->ref().light().id();
    Int32 n = 0;

    if (entity->ref().light().baseTypeOf(TypeRef::hub())) {
        for (auto cit = m_hubsOrder.begin(); cit != m_hubsOrder.end(); ++cit) {
            if ((*cit) == id) {
                return n;
            }

            ++n;
        }
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

o3d::Bool Project::deletable() const
{
    return m_deferredDelete.empty();
}

o3d::String Project::filename() const
{
    return m_path.makeFullFileName("project.o3dstudio");
}

const o3d::DiskDir &Project::path() const
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

o3d::Bool Project::setLocation(const DiskDir &path)
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
    return m_path.exists() && m_path.check("project.o3dstudio") == Dir::SUCCESS;
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

void Project::addHub(Hub *hub, Int32 index)
{
    // not created for this project
    if (hub->ref().light().projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a hub that is created for another project"))));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added hub, or with a similar id"))));
    }

    UInt64 hubId = hub->ref().light().id();
    addEntity(hub);

    // position index
    if (index >= 0) {
        Int32 n = 0;
        auto it = m_hubsOrder.begin();
        while (n < index) {
            if (it == m_hubsOrder.end()) {
                break;
            }

            ++n;
            ++it;
        }

        m_hubsOrder.emplace(it, hubId);
    } else {
        m_hubsOrder.push_back(hubId);
    }

    m_hubs[hubId] = hub;

    hub->setProject(this);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectHubAdded(hub->ref().light());
}

void Project::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove a reference for another project"))));
    }

    UInt64 hubId = _ref.id();

    auto it = m_hubs.find(hubId);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;
    m_hubs.erase(it);

    // erase its order
    auto it2 = std::find(m_hubsOrder.begin(), m_hubsOrder.end(), hub->ref().light().id());
    if (it2 != m_hubsOrder.end()) {
        m_hubsOrder.erase(it2);
    }

    // structure change
    setDirty();

    // remove from project, deferred deletion...
    removeEntity(hub);

    // signal throught workspace
    workspace()->onProjectHubRemoved(_ref);
}

void Project::removeHub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;
    LightRef _ref = hub->ref().light();
    m_hubs.erase(it);

    // erase its order
    auto it2 = std::find(m_hubsOrder.begin(), m_hubsOrder.end(), id);
    if (it2 != m_hubsOrder.end()) {
        m_hubsOrder.erase(it2);
    }

    // structure change
    setDirty();

    // remove from project, deferred deletion...
    removeEntity(hub);

    // signal throught workspace
    workspace()->onProjectHubRemoved(_ref);
}

void Project::removeHub(Hub *hub)
{
    if (!hub) {
        return;
    }

    UInt64 hubId = hub->ref().light().id();
    auto it = m_hubs.find(hubId);

    if (it != m_hubs.end()) {
        LightRef _ref = hub->ref().light();
        m_hubs.erase(it);

        // erase its order
        auto it2 = std::find(m_hubsOrder.begin(), m_hubsOrder.end(), hubId);
        if (it2 != m_hubsOrder.end()) {
            m_hubsOrder.erase(it2);
        }

        // structure change
        setDirty();

        // remove from project, deferred deletion...
        removeEntity(hub);

        // signal throught workspace
        workspace()->onProjectHubRemoved(_ref);

        return;
    }
}

Hub* Project::hub(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto it = m_hubs.find(_ref.id());
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Project::hub(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_hubs.find(_ref.id());
    if (cit != m_hubs.cend()) {
        return cit->second;
    }

    return nullptr;
}

Hub* Project::hub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Project::hub(UInt64 id) const
{
    auto cit = m_hubs.find(id);
    if (cit != m_hubs.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<Hub *> Project::searchHub(const String &name)
{
    std::list<Hub*> results;

    Hub *hub;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        if (hub->name() == name) {
            results.push_back(hub);
        }
    }

    return results;
}

std::list<const Hub*> Project::searchHub(const String &name) const
{
    std::list<const Hub*> results;

    const Hub *hub;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        if (hub->name() == name) {
            results.push_back(hub);
        }
    }

    return results;
}

Hub *Project::findHub(UInt64 id)
{
    // first level
    Hub *result = nullptr;
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        result = hub->findHub(id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Project::findHub(UInt64 id) const
{
    // first level
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        result = hub->findHub(id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

Hub *Project::findHub(const Uuid &uuid)
{
    // first level
    Hub *result = nullptr;
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        result = hub->findHub(uuid);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Project::findHub(const Uuid &uuid) const
{
    // first level
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        result = hub->findHub(uuid);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

size_t Project::numHubs() const
{
    return m_hubs.size();
}

std::list<Hub*> Project::hubs(Bool recurse)
{
    // first level
    std::list<Hub*> results;
    std::list<Hub*> childResults;
    Hub *hub = nullptr;

    if (recurse) {
        for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
            hub = it->second;

            results.push_back(hub);

            childResults = hub->hubs(recurse);
            results.insert(results.end(), childResults.begin(), childResults.end());
        }
    } else {
        for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
            hub = it->second;

            results.push_back(hub);
        }
    }

    return results;
}

std::list<const Hub *> Project::hubs(Bool recurse) const
{
    // first level
    std::list<const Hub*> results;
    std::list<const Hub*> childResults;
    const Hub *hub = nullptr;

    if (recurse) {
        for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
            hub = cit->second;

            results.push_back(hub);

            childResults = hub->hubs(recurse);
            results.insert(results.end(), childResults.begin(), childResults.end());
        }
    } else {
        for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
            hub = cit->second;

            results.push_back(hub);
        }
    }

    return results;
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

    // remove from project, deferred deletion...
    removeEntity(fragment);

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

    // remove from project, deferred deletion...
    removeEntity(fragment);

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

        // remove from project, deferred deletion...
        removeEntity(fragment);

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

void Project::addAsset(Asset *asset)
{
    // not created for this project
    if (asset->ref().light().projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add an asset that is created for another project"))));
    }

    // already exists
    if (m_assets.find(asset->ref().light().id()) != m_assets.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added asset, or with a similar id"))));
    }

    m_assets[asset->ref().light().id()] = asset;
    addEntity(asset);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectAssetAdded(asset->ref().light());
}

void Project::removeAsset(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove a reference for another project"))));
    }

    auto it = m_assets.find(_ref.id());
    if (it == m_assets.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Asset *asset = it->second;
    m_assets.erase(it);

    // structure change
    setDirty();

    // remove from project, deferred deletion...
    removeEntity(asset);

    // signal throught workspace
    workspace()->onProjectAssetRemoved(_ref);
}

void Project::removeAsset(UInt64 id)
{
    auto it = m_assets.find(id);
    if (it == m_assets.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Asset *asset = it->second;
    LightRef _ref = asset->ref().light();
    m_assets.erase(it);

    // structure change
    setDirty();

    // remove from project, deferred deletion...
    removeEntity(asset);

    // signal throught workspace
    workspace()->onProjectAssetRemoved(_ref);
}

void Project::removeAsset(Asset *asset)
{
    if (!asset) {
        return;
    }

    UInt64 assetId = asset->ref().light().id();
    auto it = m_assets.find(assetId);

    if (it != m_assets.end()) {
        LightRef _ref = asset->ref().light();
        m_assets.erase(it);

        // structure change
        setDirty();

        // remove from project, deferred deletion...
        removeEntity(asset);

        // signal throught workspace
        workspace()->onProjectAssetRemoved(_ref);

        return;
    }
}

Asset *Project::asset(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto it = m_assets.find(_ref.id());
    if (it != m_assets.end()) {
        return it->second;
    }

    return nullptr;
}

const Asset *Project::asset(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_assets.find(_ref.id());
    if (cit != m_assets.cend()) {
        return cit->second;
    }

    return nullptr;
}

Asset *Project::asset(UInt64 id)
{
    auto it = m_assets.find(id);
    if (it != m_assets.end()) {
        return it->second;
    }

    return nullptr;
}

const Asset *Project::asset(UInt64 id) const
{
    auto cit = m_assets.find(id);
    if (cit != m_assets.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<Asset *> Project::searchAsset(const String &name)
{
    std::list<Asset*> results;

    Asset *asset;
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        asset = it->second;

        if (asset->name() == name) {
            results.push_back(asset);
        }
    }

    return results;
}

std::list<const Asset *> Project::searchAsset(const String &name) const
{
    std::list<const Asset*> results;

    const Asset *asset;
    for (auto cit = m_assets.cbegin(); cit != m_assets.cend(); ++cit) {
        asset = cit->second;

        if (asset->name() == name) {
            results.push_back(asset);
        }
    }

    return results;
}

std::list<Asset *> Project::assets()
{
    // first level
    std::list<Asset*> results;
    Asset *asset= nullptr;

    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        asset = it->second;
        results.push_back(asset);
    }

    return results;
}

std::list<const Asset *> Project::assets() const
{
    // first level
    std::list<const Asset*> results;
    const Asset *asset = nullptr;

    for (auto cit = m_assets.cbegin(); cit != m_assets.cend(); ++cit) {
        asset = cit->second;
        results.push_back(asset);
    }

    return results;
}

void Project::addEntity(Entity *entity)
{
    if (!entity) {
        return;
    }

    UInt64 id = entity->ref().light().id();

    m_entitiesById[id] = entity;
    m_entitiesByUuid[entity->ref().uuid()] = entity;

    if (entity->ref().light().baseTypeOf(TypeRef::hub())) {
        // sync with the master scene
        SceneCommand *sceneCommand = new SceneHubCommand(static_cast<Hub*>(entity), SceneHubCommand::CREATE);
        m_masterScene->addCommand(sceneCommand);
    }
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

    if (entity->ref().light().baseTypeOf(TypeRef::hub())) {
        // sync with the master scene
        SceneCommand *sceneCommand = new SceneHubCommand(static_cast<Hub*>(entity), SceneHubCommand::DELETE);
        m_masterScene->addCommand(sceneCommand);
    }

    // and set as deferred delete
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

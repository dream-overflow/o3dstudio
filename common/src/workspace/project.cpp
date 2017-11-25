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
    delete m_masterScene;
    delete m_projectFile;
    delete m_info;

    // first the fragments
    Fragment *fragment;
    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {
        fragment = it->second;
        deletePtr(fragment);
    }

    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        deletePtr(hub);
    }

    Asset *asset = nullptr;
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        asset = it->second;
        deletePtr(asset);
    }
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
    if (ref.projectId() == ref.id()) {
        auto it = m_entitiesById.find(ref.id());
        if (it != m_entitiesById.end()) {
            return it->second;
        }
    }

    return nullptr;
}

const Entity *Project::lookup(const LightRef &ref) const
{
    if (ref.projectId() == ref.id()) {
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

void Project::addHub(Hub *hub)
{
    // not created for this project
    if (hub->ref().light().projectId() != ref().light().id()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a hub that is created for another project"))));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added hub, or with a similar id"))));
    }

    m_hubs[hub->ref().light().id()] = hub;
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

    auto it = m_hubs.find(_ref.id());
    if (it == m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;

    delete hub;
    m_hubs.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectHubRemoved(hub->ref().light());
}

void Project::removeHub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;

    delete hub;
    m_hubs.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectHubRemoved(hub->ref().light());
}

void Project::removeHub(Hub *hub)
{
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        if (it->second == hub) {
            delete it->second;
            m_hubs.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            workspace()->onProjectHubRemoved(hub->ref().light());

            return;
        }
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

    delete fragment;
    m_fragments.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectFragmentRemoved(fragment->ref().light());
}

void Project::removeFragment(UInt64 id)
{
    auto it = m_fragments.find(id);
    if (it == m_fragments.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Fragment *fragment = it->second;

    delete fragment;
    m_fragments.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectFragmentRemoved(fragment->ref().light());
}

void Project::removeFragment(Fragment *fragment)
{
    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {   
        if (it->second == fragment) {
            delete it->second;
            m_fragments.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            workspace()->onProjectFragmentRemoved(fragment->ref().light());

            return;
        }
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
    if (m_fragments.find(asset->ref().light().id()) != m_fragments.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added asset, or with a similar id"))));
    }

    m_assets[asset->ref().light().id()] = asset;

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

    delete asset;
    m_assets.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectAssetRemoved(asset->ref().light());
}

void Project::removeAsset(UInt64 id)
{
    auto it = m_assets.find(id);
    if (it == m_assets.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Asset *asset = it->second;

    delete asset;
    m_assets.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    workspace()->onProjectAssetRemoved(asset->ref().light());
}

void Project::removeAsset(Asset *asset)
{
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        if (it->second == asset) {
            delete it->second;
            m_assets.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            workspace()->onProjectAssetRemoved(asset->ref().light());

            return;
        }
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

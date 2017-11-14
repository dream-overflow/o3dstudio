/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/storage/store.h"

#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/projectinfo.h"
#include "o3d/studio/common/workspace/projectfile.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;

Project::Project(const QString &name, Workspace *workspace) :
    Entity(nullptr),
    m_workspace(workspace),
    m_filename(),
    m_path(QDir::current()),
    m_nextId(1),
    m_info(nullptr),
    m_masterScene(nullptr)
{
    m_typeRef = TypeRef::project();

    m_name = name;
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
    Fragment *fragment = nullptr;
    foreach (fragment, m_fragments) {
        delete fragment;
    }

    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        delete hub;
    }

    Asset *asset = nullptr;
    foreach (asset, m_assets) {
        delete asset;
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

qint64 Project::generateId()
{
    qint64 nextId = m_nextId++;
    return nextId;
}

void Project::create()
{
    if (exists()) {
        throw ProjectException(tr("Project already exists"));
    };

    // project structure
    Application::instance()->store().initProject(this);

    // and project file
    m_projectFile->create();

    // never saved
    setDirty();
}

QString Project::filename() const
{
    return m_path.absoluteFilePath("project.o3dstudio");
}

const QDir &Project::path() const
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

bool Project::setLocation(const QDir &path)
{
    if (!path.exists()) {
        return false;
    }

    m_path = path.absolutePath() + QDir::separator() + m_name;

    return true;
}

bool Project::load()
{
    if (!exists()) {
        throw ProjectException(tr("Project doesn't exists"));
    };

    // project structure
    Application::instance()->store().loadProject(this);

    // project file
    m_projectFile->load();

    // store indexe
    // @todo

    return true;
}

bool Project::save()
{
    if (!exists()) {
        throw ProjectException(tr("Project doesn't exists"));
    };

    // project structure
    Application::instance()->store().saveProject(this);

    // and project file
    m_projectFile->save();

    // store indexe
    // @todo

    return true;
}

bool Project::exists() const
{
    return m_path.exists() && m_path.exists("project.o3dstudio");
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

void Project::addHub(Hub *hub)
{
    // not created for this project
    if (hub->ref().light().projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to add a hub that is created for another project"));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        throw ProjectException(tr("Trying to add a previously added hub, or with a similar id"));
    }

    m_hubs.insert(hub->ref().light().id(), hub);
    hub->setProject(this);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectHubAdded(hub->ref().light());
}

void Project::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to remove a reference for another project"));
    }

    auto it = m_hubs.find(_ref.id());
    if (it == m_hubs.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Hub *hub = it.value();

    delete hub;
    m_hubs.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectHubRemoved(hub->ref().light());
}

void Project::removeHub(qint64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Hub *hub = it.value();

    delete hub;
    m_hubs.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectHubRemoved(hub->ref().light());
}

void Project::removeHub(Hub *hub)
{
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        if (it.value() == hub) {       
            delete it.value();
            m_hubs.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            emit workspace()->onProjectHubRemoved(hub->ref().light());

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
        return it.value();
    }

    return nullptr;
}

const Hub* Project::hub(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_hubs.constFind(_ref.id());
    if (cit != m_hubs.cend()) {
        return cit.value();
    }

    return nullptr;
}

Hub* Project::hub(qint64 id)
{
    auto it = m_hubs.find(id);
    if (it != m_hubs.end()) {
        return it.value();
    }

    return nullptr;
}

const Hub* Project::hub(qint64 id) const
{
    auto cit = m_hubs.constFind(id);
    if (cit != m_hubs.cend()) {
        return cit.value();
    }

    return nullptr;
}

QList<Hub*> Project::searchHub(const QString &name)
{
    QList<Hub*> results;

    Hub *hub;
    foreach (hub, m_hubs) {
        if (hub->name() == name) {
            results.append(hub);
        }
    }

    return results;
}

QList<const Hub*> Project::searchHub(const QString &name) const
{
    QList<const Hub*> results;

    const Hub *hub;
    foreach (hub, m_hubs) {
        if (hub->name() == name) {
            results.append(hub);
        }
    }

    return results;
}

Hub *Project::findHub(qint64 id)
{
    // first level
    Hub *result = nullptr;
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Project::findHub(qint64 id) const
{
    // first level
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

Hub *Project::findHub(const QUuid &uuid)
{
    // first level
    Hub *result = nullptr;
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(uuid);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Project::findHub(const QUuid &uuid) const
{
    // first level
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(uuid);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

int Project::numHubs() const
{
    return m_hubs.size();
}

QList<Hub*> Project::hubs(bool recurse)
{
    // first level
    QList<Hub*> results;
    Hub *hub = nullptr;

    if (recurse) {
        foreach (hub, m_hubs) {
            results.append(hub);
            results += hub->hubs(recurse);
        }
    } else {
        foreach (hub, m_hubs) {
            results += hub;
        }
    }

    return results;
}

QList<const Hub *> Project::hubs(bool recurse) const
{
    // first level
    QList<const Hub*> results;
    const Hub *hub = nullptr;

    if (recurse) {
        foreach (hub, m_hubs) {
            results.append(hub);
            results += hub->hubs(recurse);
        }
    } else {
        foreach (hub, m_hubs) {
            results += hub;
        }
    }

    return results;
}

void Project::addFragment(Fragment *fragment)
{
    // not created for this project
    if (fragment->ref().light().projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to add a fragment that is created for another project"));
    }

    // already exists
    if (m_fragments.find(fragment->ref().light().id()) != m_fragments.end()) {
        throw ProjectException(tr("Trying to add a previously added fragment, or with a similar id"));
    }

    m_fragments.insert(fragment->ref().light().id(), fragment);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectFragmentAdded(fragment->ref().light());
}

void Project::removeFragment(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to remove a reference for another project"));
    }

    auto it = m_fragments.find(_ref.id());
    if (it == m_fragments.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Fragment *fragment = it.value();

    delete fragment;
    m_fragments.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectFragmentRemoved(fragment->ref().light());
}

void Project::removeFragment(qint64 id)
{
    auto it = m_fragments.find(id);
    if (it == m_fragments.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Fragment *fragment = it.value();

    delete fragment;
    m_fragments.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectFragmentRemoved(fragment->ref().light());
}

void Project::removeFragment(Fragment *fragment)
{
    for (auto it = m_fragments.begin(); it != m_fragments.end(); ++it) {
        if (it.value() == fragment) {
            delete it.value();
            m_fragments.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            emit workspace()->onProjectFragmentRemoved(fragment->ref().light());

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
        return it.value();
    }

    return nullptr;
}

const Fragment *Project::fragment(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_fragments.constFind(_ref.id());
    if (cit != m_fragments.cend()) {
        return cit.value();
    }

    return nullptr;
}

Fragment *Project::fragment(qint64 id)
{
    auto it = m_fragments.find(id);
    if (it != m_fragments.end()) {
        return it.value();
    }

    return nullptr;
}

const Fragment *Project::fragment(qint64 id) const
{
    auto cit = m_fragments.constFind(id);
    if (cit != m_fragments.cend()) {
        return cit.value();
    }

    return nullptr;
}

QList<Fragment *> Project::searchFragment(const QString &name)
{
    QList<Fragment*> results;

    Fragment *fragment;
    foreach (fragment, m_fragments) {
        if (fragment->name() == name) {
            results.append(fragment);
        }
    }

    return results;
}

QList<const Fragment *> Project::searchFragment(const QString &name) const
{
    QList<const Fragment*> results;

    const Fragment *fragment;
    foreach (fragment, m_fragments) {
        if (fragment->name() == name) {
            results.append(fragment);
        }
    }

    return results;
}

QList<Fragment *> Project::fragments()
{
    // first level
    QList<Fragment*> results;
    Fragment *fragment = nullptr;

    foreach (fragment, m_fragments) {
        results.append(fragment);
    }

    return results;
}

QList<const Fragment *> Project::fragments() const
{
    // first level
    QList<const Fragment*> results;
    const Fragment *fragment = nullptr;

    foreach (fragment, m_fragments) {
        results.append(fragment);
    }

    return results;
}

void Project::addAsset(Asset *asset)
{
    // not created for this project
    if (asset->ref().light().projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to add an asset that is created for another project"));
    }

    // already exists
    if (m_fragments.find(asset->ref().light().id()) != m_fragments.end()) {
        throw ProjectException(tr("Trying to add a previously added asset, or with a similar id"));
    }

    m_assets.insert(asset->ref().light().id(), asset);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectAssetAdded(asset->ref().light());
}

void Project::removeAsset(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().id()) {
        throw ProjectException(tr("Trying to remove a reference for another project"));
    }

    auto it = m_assets.find(_ref.id());
    if (it == m_assets.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Asset *asset = it.value();

    delete asset;
    m_assets.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectAssetRemoved(asset->ref().light());
}

void Project::removeAsset(qint64 id)
{
    auto it = m_assets.find(id);
    if (it == m_assets.end()) {
        throw ProjectException(tr("Trying to remove an unknown reference"));
    }

    Asset *asset = it.value();

    delete asset;
    m_assets.erase(it);

    // structure change
    setDirty();

    // signal throught workspace
    emit workspace()->onProjectAssetRemoved(asset->ref().light());
}

void Project::removeAsset(Asset *asset)
{
    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        if (it.value() == asset) {
            delete it.value();
            m_assets.erase(it);

            // structure change
            setDirty();

            // signal throught workspace
            emit workspace()->onProjectAssetRemoved(asset->ref().light());

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
        return it.value();
    }

    return nullptr;
}

const Asset *Project::asset(const LightRef &_ref) const
{
    if (_ref.projectId() != ref().light().id()) {
        return nullptr;
    }

    auto cit = m_assets.constFind(_ref.id());
    if (cit != m_assets.cend()) {
        return cit.value();
    }

    return nullptr;
}

Asset *Project::asset(qint64 id)
{
    auto it = m_assets.find(id);
    if (it != m_assets.end()) {
        return it.value();
    }

    return nullptr;
}

const Asset *Project::asset(qint64 id) const
{
    auto cit = m_assets.constFind(id);
    if (cit != m_assets.cend()) {
        return cit.value();
    }

    return nullptr;
}

QList<Asset *> Project::searchAsset(const QString &name)
{
    QList<Asset*> results;

    Asset *asset;
    foreach (asset, m_assets) {
        if (asset->name() == name) {
            results.append(asset);
        }
    }

    return results;
}

QList<const Asset *> Project::searchAsset(const QString &name) const
{
    QList<const Asset*> results;

    const Asset *asset;
    foreach (asset, m_assets) {
        if (asset->name() == name) {
            results.append(asset);
        }
    }

    return results;
}

QList<Asset *> Project::assets()
{
    // first level
    QList<Asset*> results;
    Asset *asset= nullptr;

    foreach (asset, m_assets) {
        results.append(asset);
    }

    return results;
}

QList<const Asset *> Project::assets() const
{
    // first level
    QList<const Asset*> results;
    const Asset *asset = nullptr;

    foreach (asset, m_assets) {
        results.append(asset);
    }

    return results;
}

ProjectException::ProjectException(const QString &message) :
    BaseException(message)
{

}

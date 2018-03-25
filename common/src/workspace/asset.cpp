/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/dummyhub.h"

using namespace o3d::studio::common;


Asset::Asset(const String &name, Entity *parent) :
    Entity(name, parent)
{
    m_typeRef = TypeRef::asset();
    m_ref = ObjectRef(TypeRef::asset());
}

Asset::~Asset()
{
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        delete hub;
    }
}

Entity::Role Asset::role() const
{
    return ROLE_ASSET;
}

void Asset::setProject(Project *project)
{
    m_parent = project;
}

Project *Asset::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Asset::project() const
{
    return static_cast<Project*>(m_parent);
}

o3d::Int32 Asset::childIndexOf(Entity *entity) const
{
    // @todo
    return -1;
}

void Asset::create()
{
    setDirty();
}

void Asset::destroy()
{

}

void Asset::update()
{

}

o3d::Bool Asset::deletable() const
{
    return True;
}

o3d::Bool Asset::load()
{
    return True;
}

o3d::Bool Asset::save()
{
    return True;
}

o3d::Bool Asset::exists() const
{
    return Entity::exists();
}

o3d::Bool Asset::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    Int32 num = m_hubs.size();
    stream << num;

    // hubs recursively, and by order
    const Hub *hub = nullptr;
    for (auto cit = m_hubsOrder.cbegin(); cit != m_hubsOrder.cend(); ++cit) {
        auto cit2 = m_hubs.find(*cit);
        hub = cit2->second;

        // uuid and type ref, for instanciation
        stream << hub->ref().uuid()
               << hub->ref().strong().typeName()
               << *hub;
    }

    return True;
}

o3d::Bool Asset::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    Hub *hub = nullptr;

    String typeName;
    Uuid uuid;
    Int32 num = 0;
    stream >> num;

    ComponentRegistry &components = Application::instance()->components();
    Component *component = nullptr;

    for (Int32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        component = components.componentByTarget(typeName);
        if (component) {
            hub = component->buildHub("", project(), this);
            hub->setRef(ObjectRef::buildRef(project(), component->targetTypeRef(), uuid));
            hub->setProject(project());

            stream >> *hub;
        } else {
            hub = new DummyHub("", this);
            hub->setRef(ObjectRef::buildRef(project(), hub->typeRef(), uuid));
            hub->setProject(project());

            stream >> *hub;

            // and pass the extra data
            // stream.skipRawData(hubSize...) // @todo
        }

        UInt64 hubId = hub->ref().light().id();

        m_hubs[hubId] = hub;
        m_hubsOrder.push_back(hubId);

        project()->addEntity(hub);
    }

    return False;
}

void Asset::setCreator(const o3d::String &creator)
{
    m_creator = creator;
}

const o3d::String &Asset::creator() const
{
    return m_creator;
}

void Asset::setDescription(const o3d::String &descr)
{
    m_description = descr;
}

const o3d::String &Asset::description() const
{
    return m_description;
}

void Asset::setComment(const o3d::String &comment)
{
    m_comment = comment;
}

const o3d::String &Asset::comment()
{
    return m_comment;
}

void Asset::setVersion(o3d::UInt32 majorVers, o3d::UInt32 minorVers, o3d::UInt32 subVers)
{
    m_version[0] = majorVers;
    m_version[1] = minorVers;
    m_version[2] = subVers;
}

o3d::UInt32 Asset::version(o3d::UInt32 part) const
{
    if (part > 2) {
        O3D_ERROR(E_IndexOutOfRange("Version part index between 0..2"));
    }

    return m_version[part];
}

void Asset::setVersionStatus(Asset::VersionStatus status)
{
    m_versionStatus = status;
}

Asset::VersionStatus Asset::versionStatus() const
{
    return m_versionStatus;
}

void Asset::setRevision(o3d::UInt32 rev)
{
    m_revision = rev;
}

o3d::UInt32 Asset::revision() const
{
    return m_revision;
}

void Asset::setCreationTimeStamp(const o3d::DateTime &date)
{
    m_creationTimeStamp = date;
}

const o3d::DateTime &Asset::creationTimeStamp() const
{
    return m_creationTimeStamp;
}


void Asset::addHub(Hub *hub, Int32 index)
{
    // not created for this project
    if (hub->ref().light().projectId() != project()->ref().light().id()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to add a hub that is created for another project"))));
    }

    UInt64 hubId = hub->ref().light().id();

    // already exists
    if (m_hubs.find(hubId) != m_hubs.end()) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Trying to add a previously added hub, or with a similar id"))));
    }

    m_hubs[hubId] = hub;

    project()->addEntity(hub);

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

    hub->setProject(project());

    // structure change
    setDirty();

    // create the hub now
    hub->create();
}

void Asset::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != ref().light().projectId()) {
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
    auto it2 = std::find(m_hubsOrder.begin(), m_hubsOrder.end(), hubId);
    if (it2 != m_hubsOrder.end()) {
        m_hubsOrder.erase(it2);
    }

    // structure change
    setDirty();

    // deferred deletion
    project()->deleteEntity(hub);

    // signal throught project->workspace
    project()->workspace()->onProjectHubRemoved(_ref);
}

void Asset::removeHub(UInt64 id)
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

    // deferred deletion
    project()->deleteEntity(hub);

    // signal throught project->workspace
    project()->workspace()->onProjectHubRemoved(_ref);
}

void Asset::removeHub(Hub *hub)
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

        // deferred deletion
        project()->deleteEntity(hub);

        // signal throught project->workspace
        project()->workspace()->onProjectHubRemoved(_ref);

        return;
    }
}

Hub* Asset::hub(const LightRef &_ref)
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

const Hub* Asset::hub(const LightRef &_ref) const
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

Hub* Asset::hub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Asset::hub(UInt64 id) const
{
    auto cit = m_hubs.find(id);
    if (cit != m_hubs.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<Hub *> Asset::searchHub(const String &name)
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

std::list<const Hub*> Asset::searchHub(const String &name) const
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

Hub *Asset::findHub(UInt64 id)
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

const Hub *Asset::findHub(UInt64 id) const
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

Hub *Asset::findHub(const Uuid &uuid)
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

const Hub *Asset::findHub(const Uuid &uuid) const
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

size_t Asset::numHubs() const
{
    return m_hubs.size();
}

std::list<Hub*> Asset::hubs(Bool recurse)
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

std::list<const Hub *> Asset::hubs(Bool recurse) const
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

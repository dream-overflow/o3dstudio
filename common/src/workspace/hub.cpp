/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/dummyhub.h"

using namespace o3d::studio::common;


Hub::Hub(const String &name, Entity *parent) :
    Entity(name, parent),
    m_project(nullptr)
{
    m_typeRef = TypeRef::hub();
    m_ref = ObjectRef(TypeRef::hub());

    if (parent) {
        // auto set project from parent
        m_project = parent->project();
    }
}

Hub::~Hub()
{
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        // remove the hub reference
        project()->removeEntity(hub);

        // safely delete now
        delete hub;
    }
}

void Hub::setProject(Project *project)
{
    m_project = project;
}

Project *Hub::project()
{
    return m_project;
}

const Project *Hub::project() const
{
    return m_project;
}

void Hub::create()
{
    setDirty();

    project()->addEntity(this);

    // signal throught project->workspace
    project()->workspace()->onProjectHubAdded(ref().light());
}

void Hub::destroy()
{
    // recursive destroy, because of the order, leaves before
    Hub *hub;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        hub->destroy();
    }

    // signal throught project->workspace
    project()->workspace()->onProjectHubRemoved(ref().light());
}

o3d::Bool Hub::load()
{
    return True;
}

o3d::Bool Hub::save()
{
    return True;
}

o3d::Bool Hub::exists() const
{
    return Entity::exists();
}

o3d::Bool Hub::deletable() const
{
    Hub *hub;
    // undeletable before all children are deletables
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        if (!hub->deletable()) {
            return False;
        }
    }

    return True;
}

void Hub::addHub(Hub *hub, Int32 index)
{
    // not created for this project
    if (hub->ref().light().projectId() != project()->ref().light().id()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to add a hub that is created for another project"))));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to add a previously added hub, or with a similar id"))));
    }

    UInt64 hubId = hub->ref().light().id();
    m_hubs[hubId] = hub;

    // called by create below
    // project()->addEntity(hub);

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

    // structure change (called by create)
    // setDirty();

    // create the hub now
    hub->create();
}

void Hub::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != project()->ref().light().id()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove a reference for another project"))));
    }

    UInt64 hubId = _ref.id();

    auto it = m_hubs.find(hubId);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove an unknown reference"))));
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

    // recursively remove its children and related entities
    hub->destroy();

    // add for a deferred deletion
    project()->deleteEntity(hub);
}

void Hub::removeHub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove an unknown reference"))));
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

    // recursively remove its children and related entities
    hub->destroy();

    // add for a deferred deletion
    project()->deleteEntity(hub);
}

void Hub::removeHub(Hub *hub)
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

        setDirty();

        hub->destroy();

        // add for a deferred deletion
        project()->deleteEntity(hub);

        return;
    }
}

Hub* Hub::hub(const LightRef &_ref)
{
    if (_ref.projectId() != project()->ref().light().id()) {
        return nullptr;
    }

    auto it = m_hubs.find(_ref.id());
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Hub::hub(const LightRef &_ref) const
{
    if (_ref.projectId() != project()->ref().light().id()) {
        return nullptr;
    }

    auto cit = m_hubs.find(_ref.id());
    if (cit != m_hubs.cend()) {
        return cit->second;
    }

    return nullptr;
}

Hub* Hub::hub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

const Hub* Hub::hub(UInt64 id) const
{
    auto cit = m_hubs.find(id);
    if (cit != m_hubs.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<Hub*> Hub::searchHub(const String &name)
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

std::list<const Hub*> Hub::searchHub(const String &name) const
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

Hub *Hub::findHub(UInt64 id)
{
    // himself
    if (id == m_ref.light().id()) {
        return this;
    }

    // recurse on children
    Hub *result = nullptr;
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        result = hub->findHub(id);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Hub::findHub(UInt64 id) const
{
    // himself
    if (id == m_ref.light().id()) {
        return this;
    }

    // recurse on children
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        result = hub->findHub(id);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

Hub *Hub::findHub(const Uuid &uuid)
{
    // himself
    if (uuid == m_ref.uuid()) {
        return this;
    }

    // recurse on children
    Hub *result = nullptr;
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;

        result = hub->findHub(uuid);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Hub::findHub(const Uuid &uuid) const
{
    // himself
    if (uuid == m_ref.uuid()) {
        return this;
    }

    // recurse on children
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        result = hub->findHub(uuid);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

size_t Hub::numHubs() const
{
    return m_hubs.size();
}

std::list<Hub *> Hub::hubs(Bool recurse)
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

std::list<const Hub *> Hub::hubs(Bool recurse) const
{
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

o3d::Int32 Hub::childIndexOf(Entity *entity) const
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

void Hub::removeFromScene(MasterScene *masterScene)
{
    // remove recursively all children, starting by leaves
    for (auto it1 = m_hubs.begin(); it1 != m_hubs.end(); ++it1) {
        it1->second->removeFromScene(masterScene);
    }
}

o3d::Bool Hub::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    Int32 num = m_hubs.size();
    stream << num;

    // children recursively, and by order
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

o3d::Bool Hub::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    Hub *hub = nullptr;

    // setup now
    create();

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
            hub = component->buildHub("", m_project, this);
            hub->setRef(ObjectRef::buildRef(m_project, component->targetTypeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;
        } else {
            hub = new DummyHub("", this);
            hub->setRef(ObjectRef::buildRef(m_project, hub->typeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;

            // and pass the extra data
            // stream.skipRawData(hubSize...) // @todo
        }

        UInt64 hubId = hub->ref().light().id();

        m_hubs[hubId] = hub;
        m_hubsOrder.push_back(hubId);

        project()->addEntity(hub);
    }

    return True;
}

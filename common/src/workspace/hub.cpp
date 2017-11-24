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
}

Hub::~Hub()
{
    Hub *hub = nullptr;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
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
}

o3d::Bool Hub::load()
{
    return true;
}

o3d::Bool Hub::save()
{
    return true;
}

o3d::Bool Hub::exists() const
{
    return Entity::exists();
}

void Hub::addHub(Hub *hub)
{
    // not created for this project
    if (hub->ref().light().projectId() != project()->ref().light().id()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to add a hub that is created for another project"))));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to add a previously added hub, or with a similar id"))));
    }

    m_hubs[hub->ref().light().id()] = hub;
    hub->setProject(project());

    setDirty();

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubAdded(hub->ref().light());
}

void Hub::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != project()->ref().light().id()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove a reference for another project"))));
    }

    auto it = m_hubs.find(_ref.id());
    if (it == m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;

    delete hub;
    m_hubs.erase(it);

    setDirty();

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubRemoved(hub->ref().light());
}

void Hub::removeHub(UInt64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        O3D_ERROR(E_HubException(fromQString(tr("Trying to remove an unknown reference"))));
    }

    Hub *hub = it->second;

    delete hub;
    m_hubs.erase(it);

    setDirty();

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubRemoved(hub->ref().light());
}

void Hub::removeHub(Hub *hub)
{
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        if (it->second == hub) {
            delete it->second;
            m_hubs.erase(it);

            setDirty();

            // signal throught project->workspace
            project()->workspace()->onProjectHubRemoved(hub->ref().light());

            return;
        }
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

o3d::Bool Hub::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    Int32 num = m_hubs.size();
    stream << num;

    // children recursively
    const Hub *hub = nullptr;
    for (auto cit = m_hubs.cbegin(); cit != m_hubs.cend(); ++cit) {
        hub = cit->second;

        // uuid and type ref, for instanciation
        stream << ref().uuid()
               << ref().strong().typeName()
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

        m_hubs[hub->ref().light().id()] = hub;
    }

    return True;
}

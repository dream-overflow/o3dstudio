/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

using namespace o3d::studio::common;


Hub::Hub(const QString &name, Entity *parent) :
    Entity(name, parent),
    m_project(nullptr)
{
    m_ref = ObjectRef(TypeRef::hub());
}

Hub::~Hub()
{
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
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

}

bool Hub::load()
{
    return true;
}

bool Hub::save()
{
    return true;
}

bool Hub::exists() const
{
    return false;
}

bool Hub::hasChanges()
{
    return false;
}

void Hub::addHub(Hub *hub)
{
    // not created for this project
    if (hub->ref().light().projectId() != project()->ref().light().id()) {
        throw HubException(tr("Trying to add a hub that is created for another project"));
    }

    // already exists
    if (m_hubs.find(hub->ref().light().id()) != m_hubs.end()) {
        throw HubException(tr("Trying to add a previously added hub, or with a similar id"));
    }

    m_hubs.insert(hub->ref().light().id(), hub);
    hub->setProject(project());

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubAdded(hub->ref().light());
}

void Hub::removeHub(const LightRef &_ref)
{
    if (_ref.projectId() != project()->ref().light().id()) {
        throw HubException(tr("Trying to remove a reference for another project"));
    }

    auto it = m_hubs.find(_ref.id());
    if (it == m_hubs.end()) {
        throw HubException(tr("Trying to remove an unknown reference"));
    }

    Hub *hub = it.value();

    delete hub;
    m_hubs.erase(it);

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubRemoved(hub->ref().light());
}

void Hub::removeHub(qint64 id)
{
    auto it = m_hubs.find(id);
    if (it == m_hubs.end()) {
        throw HubException(tr("Trying to remove an unknown reference"));
    }

    Hub *hub = it.value();

    delete hub;
    m_hubs.erase(it);

    // signal throught project->workspace
    emit project()->workspace()->onProjectHubRemoved(hub->ref().light());
}

void Hub::removeHub(Hub *hub)
{
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        if (it.value() == hub) {
            delete it.value();
            m_hubs.erase(it);

            // signal throught project->workspace
            emit project()->workspace()->onProjectHubRemoved(hub->ref().light());

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
        return it.value();
    }

    return nullptr;
}

const Hub* Hub::hub(const LightRef &_ref) const
{
    if (_ref.projectId() != project()->ref().light().id()) {
        return nullptr;
    }

    auto cit = m_hubs.constFind(_ref.id());
    if (cit != m_hubs.cend()) {
        return cit.value();
    }

    return nullptr;
}

Hub* Hub::hub(qint64 id)
{
    auto it = m_hubs.find(id);
    if (it != m_hubs.end()) {
        return it.value();
    }

    return nullptr;
}

const Hub* Hub::hub(qint64 id) const
{
    auto cit = m_hubs.constFind(id);
    if (cit != m_hubs.cend()) {
        return cit.value();
    }

    return nullptr;
}

QList<Hub*> Hub::searchHub(const QString &name)
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

QList<const Hub*> Hub::searchHub(const QString &name) const
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

Hub *Hub::findHub(qint64 id)
{
    // himself
    if (id == m_ref.light().id()) {
        return this;
    }

    // recurse on children
    Hub *result = nullptr;
    Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(id);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

const Hub *Hub::findHub(qint64 id) const
{
    // himself
    if (id == m_ref.light().id()) {
        return this;
    }

    // recurse on children
    const Hub *result = nullptr;
    const Hub *hub = nullptr;
    foreach (hub, m_hubs) {
        result = hub->findHub(id);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

int Hub::numHubs() const
{
    return m_hubs.size();
}

HubException::HubException(const QString &message) :
    BaseException(message)
{

}

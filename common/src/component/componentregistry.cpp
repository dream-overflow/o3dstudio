/**
 * @brief Common component registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/component/componentregistry.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/workspace/hub.h"

using namespace o3d::studio::common;


ComponentRegistry::ComponentRegistry(QObject *parent) :
    QObject(parent)
{

}

ComponentRegistry::~ComponentRegistry()
{
    TypeRegistry &types = Application::instance()->types();

    m_componentsById.clear();

    Component *component = nullptr;
    foreach (component, m_components) {
        types.unregisterType(component->typeRef());
        types.unregisterType(component->hubTypeRef());

        delete component;
    }
}

void ComponentRegistry::registerComponent(Component *component)
{
    if (component) {
        auto it = m_components.find(component->name());
        if (it != m_components.end()) {
            throw ComponentException(tr("Component {0} already registred").arg(component->name()));
        }

        // register the component type
        TypeRegistry &types = Application::instance()->types();
        if (!types.registerType(COMPONENT_TYPE_ID, component->name())) {
            throw ComponentException(tr("Component type {0} cannot by registered").arg(component->name()));
        }

        // and its related hub type
        if (!types.registerType(HUB_TYPE_ID, component->hubName())) {
            throw ComponentException(tr("Hub type {0} cannot by registered").arg(component->hubName()));
        }

        m_components.insert(component->name(), component);
        m_componentsById.insert(component->typeRef().id(), component);

        component->setup();

        // signal for component interested, the UI
        emit onComponentRegistered(component->name());
    }
}

bool ComponentRegistry::unregisterComponent(const QString &name)
{
    auto it = m_components.find(name);
    if (it != m_components.end()) {
        Component *component = it.value();
        m_components.erase(it);

        auto it2 = m_componentsById.find(component->typeRef().id());
        if (it2 != m_componentsById.end()) {
            m_componentsById.erase(it2);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(component->typeRef());
        types.unregisterType(component->hubTypeRef());

        emit onComponentUnregistered(name);

        return true;
    }

    return false;
}

bool ComponentRegistry::unregisterComponent(quint64 id)
{
    auto it2 = m_componentsById.find(id);
    if (it2 != m_componentsById.end()) {
        Component *component = it2.value();
        QString name = component->name();

        auto it = m_components.find(component->name());
        if (it != m_components.end()) {
            m_components.erase(it);
        }
        m_componentsById.erase(it2);

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(component->typeRef());
        types.unregisterType(component->hubTypeRef());

        delete component;

        emit onComponentUnregistered(name);

        return true;
    }

    return false;
}

bool ComponentRegistry::unregisterComponent(const TypeRef &ref)
{
    return unregisterComponent(ref.id());
}

Component *ComponentRegistry::component(const QString &name)
{
    auto it = m_components.find(name);
    if (it != m_components.end()) {
        return it.value();
    }

    return nullptr;
}

const Component *ComponentRegistry::component(const QString &name) const
{
    auto cit = m_components.constFind(name);
    if (cit != m_components.cend()) {
        return cit.value();
    }

    return nullptr;
}

Component *ComponentRegistry::component(const TypeRef &ref)
{
    auto it = m_componentsById.find(ref.id());
    if (it != m_componentsById.end()) {
        return it.value();
    }

    return nullptr;
}

const Component *ComponentRegistry::component(const TypeRef &ref) const
{
    auto cit = m_componentsById.constFind(ref.id());
    if (cit != m_componentsById.cend()) {
        return cit.value();
    }

    return nullptr;
}

ComponentException::ComponentException(const QString &message) :
    BaseException(message)
{

}

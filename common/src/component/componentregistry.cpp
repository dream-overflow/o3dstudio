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


ComponentRegistry::ComponentRegistry(BaseObject *parent) :
    BaseObject(parent)
{

}

ComponentRegistry::~ComponentRegistry()
{
    TypeRegistry &types = Application::instance()->types();

    m_componentsById.clear();
    m_componentsByTargetName.clear();

    for (auto it = m_components.begin(); it != m_components.end(); ++it) {
        Component *component = it->second;

        types.unregisterType(component->typeRef());
        types.unregisterType(component->targetTypeRef());

        delete component;
    }
}

#include "o3d/studio/common/component/dummyhub.h"
#include "o3d/studio/common/component/viewporthub.h"
#include "o3d/studio/common/component/camerahub.h"
#include "o3d/studio/common/component/lighthub.h"
#include "o3d/studio/common/component/drawerhub.h"
#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/component/meshhub.h"
#include "o3d/studio/common/component/skinhub.h"
#include "o3d/studio/common/component/boneshub.h"

void ComponentRegistry::initialize()
{
    registerComponent(new DummyHubComponent());
    registerComponent(new ViewPortComponent());
    registerComponent(new CameraHubComponent());
    registerComponent(new LightHubComponent());
    registerComponent(new DrawerHubComponent());
    registerComponent(new SpacialNodeComponent());
    registerComponent(new MeshComponent());
    registerComponent(new BonesComponent());
    registerComponent(new SkinComponent());
}

void ComponentRegistry::registerComponent(Component *component)
{
    if (component) {
        auto it = m_components.find(component->name());
        if (it != m_components.end()) {
            O3D_ERROR(E_ComponentException(fromQString(tr("Component {0} already registred").arg(toQString(component->name())))));
        }

        // register the component type
        TypeRegistry &types = Application::instance()->types();
        if (!types.registerType(COMPONENT_TYPE_ID, component->name())) {
            O3D_ERROR(E_ComponentException(fromQString(tr("Component type {0} cannot by registered").arg(toQString(component->name())))));
        }

        // and its related hub type
        if (!types.registerType(HUB_TYPE_ID, component->targetName())) {
            O3D_ERROR(E_ComponentException(fromQString(tr("Hub type {0} cannot by registered").arg(toQString(component->targetName())))));
        }

        component->setTypeRef(types.typeRef(component->name()));
        component->setTargetTypeRef(types.typeRef(component->targetName()));

        m_components[component->name()] = component;
        m_componentsById[component->typeRef().id()] = component;
        m_componentsByTargetName[component->targetName()] = component;

        component->setup();

        // signal for component interested, the UI
        onComponentRegistered(component->name());
    }
}

o3d::Bool ComponentRegistry::unregisterComponent(const String &name)
{
    auto it = m_components.find(name);
    if (it != m_components.end()) {
        Component *component = it->second;
        m_components.erase(it);

        auto it2 = m_componentsById.find(component->typeRef().id());
        if (it2 != m_componentsById.end()) {
            m_componentsById.erase(it2);
        }

        auto it3 = m_componentsByTargetName.find(component->targetName());
        if (it3 != m_componentsByTargetName.end()) {
            m_componentsByTargetName.erase(it3);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(component->typeRef());
        types.unregisterType(component->targetTypeRef());

        onComponentUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool ComponentRegistry::unregisterComponent(UInt64 id)
{
    auto it2 = m_componentsById.find(id);
    if (it2 != m_componentsById.end()) {
        Component *component = it2->second;
        String name = component->name();

        m_componentsById.erase(it2);

        auto it = m_components.find(component->name());
        if (it != m_components.end()) {
            m_components.erase(it);
        }

        auto it3 = m_componentsByTargetName.find(component->targetName());
        if (it3 != m_componentsByTargetName.end()) {
            m_componentsByTargetName.erase(it3);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(component->typeRef());
        types.unregisterType(component->targetTypeRef());

        delete component;

        onComponentUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool ComponentRegistry::unregisterComponent(const TypeRef &ref)
{
    return unregisterComponent(ref.id());
}

Component *ComponentRegistry::component(const String &name)
{
    auto it = m_components.find(name);
    if (it != m_components.end()) {
        return it->second;
    }

    return nullptr;
}

const Component *ComponentRegistry::component(const String &name) const
{
    auto cit = m_components.find(name);
    if (cit != m_components.cend()) {
        return cit->second;
    }

    return nullptr;
}

Component *ComponentRegistry::component(const TypeRef &ref)
{
    auto it = m_componentsById.find(ref.id());
    if (it != m_componentsById.end()) {
        return it->second;
    }

    return nullptr;
}

const Component *ComponentRegistry::component(const TypeRef &ref) const
{
    auto cit = m_componentsById.find(ref.id());
    if (cit != m_componentsById.cend()) {
        return cit->second;
    }

    return nullptr;
}

Component *ComponentRegistry::componentByTarget(const String &name)
{
    auto it = m_componentsByTargetName.find(name);
    if (it != m_componentsByTargetName.end()) {
        return it->second;
    }

    return nullptr;
}

const Component *ComponentRegistry::componentByTarget(const String &name) const
{
    auto cit = m_componentsByTargetName.find(name);
    if (cit != m_componentsByTargetName.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<const Component *> ComponentRegistry::componentList() const
{
    std::list<const Component*> results;

    for (auto cit = m_components.begin(); cit != m_components.end(); ++cit) {
        results.push_back(cit->second);
    }

    return results;
}

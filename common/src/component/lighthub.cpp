/**
 * @brief Common light component offering a light hub.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/light.h>

#include "o3d/studio/common/component/lighthub.h"
#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


LightHubComponent::LightHubComponent()
{
    m_name = "o3s::common::component::lighthub";
    m_targetName = "o3s::common::hub::light";

    m_label = "Light Hub";
    m_icon = ":/icons/wb_incandescent_black.svg";
//    :/icons/wb_sunny_black.svg
}

LightHubComponent::~LightHubComponent()
{

}

void LightHubComponent::setup()
{

}

Hub *LightHubComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    LightHub *lightHub = new LightHub(name, parent);
    lightHub->setTypeRef(m_targetTypeRef);

    return lightHub;
}

LightHub::LightHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_lightType(POINT_LIGHT),
    m_instances()
{

}

LightHub::~LightHub()
{
    O3D_ASSERT(m_instances.empty());
}

void LightHub::create()
{
    Hub::create();

     // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void LightHub::destroy()
{
    // recursive destroy, because of the order, leaves before
    Hub *hub;
    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
        hub = it->second;
        hub->destroy();
    }

    // signal throught project->workspace
    project()->workspace()->onProjectHubRemoved(ref().light());

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool LightHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool LightHub::load()
{
    return Hub::load();
}

o3d::Bool LightHub::save()
{
    return Hub::save();
}

o3d::Bool LightHub::exists() const
{
    return Entity::exists();
}

o3d::Bool LightHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool LightHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void LightHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Light *light = new o3d::Light(masterScene->scene());
    light->setName(m_name);

    if (m_lightType == POINT_LIGHT) {
        light->setLightType(o3d::Light::POINT_LIGHT);
    } else if (m_lightType == SPOT_LIGHT) {
        light->setLightType(o3d::Light::SPOT_LIGHT);
    } else if (m_lightType == DIRECTIONAL_LIGHT) {
        light->setLightType(o3d::Light::DIRECTIONAL_LIGHT);
    } else if (m_lightType == LIGHT_MAP) {
        light->setLightType(o3d::Light::LIGHT_MAP);
    }

    // if the parent hub is a spacial node add the light the it
    if (parent() && parent()->isParentHub() && static_cast<Hub*>(parent())->isSpacialNode()) {
        SpacialNodeHub *parentHub = static_cast<SpacialNodeHub*>(parent());
        parentHub->addChildToScene(masterScene, light);
    }

    m_instances[masterScene] = light;

    // scene object id is as the base of the pickable color id
    project()->addPickable((UInt32)light->getId(), this);

    O3D_MESSAGE("LightHub created into scene");
}

void LightHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Light *light = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)light->getId());

        light->getParent()->deleteChild(light);

        O3D_MESSAGE("LightHub deleted from scene");
    }
}

void LightHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Light *light = it->second;

        // hub => o3d
        light->setName(m_name);

        // @todo properties

        if (m_lightType == POINT_LIGHT) {
            light->setLightType(o3d::Light::POINT_LIGHT);
        } else if (m_lightType == SPOT_LIGHT) {
            light->setLightType(o3d::Light::SPOT_LIGHT);
        } else if (m_lightType == DIRECTIONAL_LIGHT) {
            light->setLightType(o3d::Light::DIRECTIONAL_LIGHT);
        } else if (m_lightType == LIGHT_MAP) {
            light->setLightType(o3d::Light::LIGHT_MAP);
        }

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("LightHub synced into scene");
    }
}

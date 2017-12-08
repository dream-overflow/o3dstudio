/**
 * @brief Common spacial node component offering a 3d node for scene graph.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/hierarchy/node.h>

#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


SpacialNodeComponent::SpacialNodeComponent()
{
    m_name = "o3s::common::component::spacialhub";
    m_targetName = "o3s::common::hub::spacialhub";

    m_label = "Spacial Node Hub";
    m_icon = ":/icons/3d_rotation_black.svg";
}

SpacialNodeComponent::~SpacialNodeComponent()
{

}

void SpacialNodeComponent::setup()
{

}

Hub *SpacialNodeComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    SpacialNodeHub *spacialNodeHub = new SpacialNodeHub(name, parent);
    spacialNodeHub->setTypeRef(m_targetTypeRef);

    return spacialNodeHub;
}

SceneUIElement *SpacialNodeComponent::sceneUiElement(SceneUIElement::Mode mode)
{
    return nullptr;
}

Panel *SpacialNodeComponent::panel(Panel::PanelType panelType)
{
    return nullptr;
}

SpacialNodeHub::SpacialNodeHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_nodePolicy(POLICY_DEFAULT),
    m_instances()
{

}

SpacialNodeHub::~SpacialNodeHub()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        o3d::Node *node= it->second;
        delete node;
    }
}

void SpacialNodeHub::create()
{

}

o3d::Bool SpacialNodeHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool SpacialNodeHub::load()
{
    return Hub::load();
}

o3d::Bool SpacialNodeHub::save()
{
    return Hub::save();
}

o3d::Bool SpacialNodeHub::exists() const
{
    return Entity::exists();
}

o3d::Bool SpacialNodeHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool SpacialNodeHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void SpacialNodeHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Node *node = new o3d::Node(masterScene->scene());
    node->setName(m_name);

    m_instances[masterScene] = node;

    O3D_MESSAGE("SpacialNodeHub created into scene");
}

void SpacialNodeHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Node *node = it->second;
        m_instances.erase(it);
        delete node;

        O3D_MESSAGE("SpacialNodeHub deleted from scene");
    }
}

void SpacialNodeHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Node *node = it->second;

        // hub => o3d
        node->setName(m_name);

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("SpacialNodeHub synced into scene");
    }
}

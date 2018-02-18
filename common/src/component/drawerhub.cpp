/**
 * @brief Common drawer component offering a drawer hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-16
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/deferred/deferreddrawer.h>
#include <o3d/engine/shadow/shadowvolumeforward.h>

#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"
#include "o3d/studio/common/component/drawerhub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


DrawerHubComponent::DrawerHubComponent() :
    Component()
{
    m_name = "o3s::common::component::drawerhub";
    m_targetName = "o3s::common::hub::drawer";

    m_label = "Scene Drawer Hub";
    m_icon = ":/icons/blur_on_black.svg";
}

DrawerHubComponent::~DrawerHubComponent()
{

}

void DrawerHubComponent::setup()
{

}

Hub *DrawerHubComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    DrawerHub *drawerHub = new DrawerHub(name, parent);
    drawerHub->setTypeRef(m_targetTypeRef);

    return drawerHub;
}

DrawerHub::DrawerHub(const String &name, Entity *parent) :
    Hub(name, parent)
{

}

DrawerHub::~DrawerHub()
{
    // O3D_ASSERT(m_instances.empty());
}

void DrawerHub::create()
{
    Hub::create();

    // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

o3d::Bool DrawerHub::deletable() const
{
    // @todo
    return True;
}

o3d::Bool DrawerHub::load()
{
    return Hub::load();
}

o3d::Bool DrawerHub::save()
{
    return Hub::save();
}

o3d::Bool DrawerHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

o3d::Bool DrawerHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool DrawerHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void DrawerHub::createToScene(MasterScene *masterScene)
{
    // @todo
}

void DrawerHub::removeFromScene(MasterScene *masterScene)
{
    // @todo
}

void DrawerHub::syncWithScene(MasterScene */*masterScene*/)
{
    // nothing to do
}

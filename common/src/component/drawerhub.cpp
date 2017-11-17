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

#include "o3d/studio/common/component/drawerhub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


DrawerHubComponent::DrawerHubComponent() :
    Component()
{
    m_name = "o3s::common::component::drawerhub";
    m_targetName = "o3s::common::hub::drawer";
}

DrawerHubComponent::~DrawerHubComponent()
{

}

void DrawerHubComponent::setup()
{

}

Hub *DrawerHubComponent::buildHub(const QString &name, Project *project, Entity *parent)
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

DrawerHub::DrawerHub(const QString &name, Entity *parent) :
    Hub(name, parent)
{

}

DrawerHub::~DrawerHub()
{

}

void DrawerHub::create()
{

}

bool DrawerHub::load()
{
    return Hub::load();
}

bool DrawerHub::save()
{
    return Hub::save();
}

bool DrawerHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

bool DrawerHub::serializeContent(QDataStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return false;
    }

    return true;
}

bool DrawerHub::deserializeContent(QDataStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return false;
    }

    return true;
}

void DrawerHub::createToScene(MasterScene *masterScene)
{

}

void DrawerHub::removeFromScene(MasterScene *masterScene)
{

}

void DrawerHub::syncWithScene(MasterScene *masterScene)
{

}

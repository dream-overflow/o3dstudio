/**
 * @brief Common static mesh component offering a 3d static mesh.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/mesh.h>

#include "o3d/studio/common/component/meshhub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


MeshComponent::MeshComponent()
{
    m_name = "o3s::common::component::meshhub";
    m_targetName = "o3s::common::hub::meshhub";

    m_label = "Static Mesh Hub";
    m_icon = ":/icons/grid_on_black.svg";
}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::setup()
{

}

Hub *MeshComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    MeshHub *meshHub = new MeshHub(name, parent);
    meshHub->setTypeRef(m_targetTypeRef);

    return meshHub;
}

SceneUIElement *MeshComponent::sceneUiElement(SceneUIElement::Mode mode)
{
    return nullptr;
}

Panel *MeshComponent::panel(Panel::PanelType panelType)
{
    return nullptr;
}

MeshHub::MeshHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_instances()
{

}

MeshHub::~MeshHub()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        o3d::Mesh *mesh= it->second;
        delete mesh;
    }
}

void MeshHub::create()
{

}

o3d::Bool MeshHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool MeshHub::load()
{
    return Hub::load();
}

o3d::Bool MeshHub::save()
{
    return Hub::save();
}

o3d::Bool MeshHub::exists() const
{
    return Entity::exists();
}

o3d::Bool MeshHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool MeshHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void MeshHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Mesh *mesh = new o3d::Mesh(masterScene->scene());
    mesh->setName(m_name);

    m_instances[masterScene] = mesh;

    O3D_MESSAGE("MeshHub created into scene");
}

void MeshHub::removeFromScene(MasterScene *masterScene)
{
    Hub::removeFromScene(masterScene);

    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Mesh *mesh = it->second;
        m_instances.erase(it);
        delete mesh;

        O3D_MESSAGE("MeshHub deleted from scene");
    }
}

void MeshHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Mesh *mesh = it->second;

        // hub => o3d
        mesh->setName(m_name);

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("MeshHub synced into scene");
    }
}

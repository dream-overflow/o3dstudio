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
#include "o3d/studio/common/component/spacialnodehub.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

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
    O3D_ASSERT(m_instances.empty());
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

#include <o3d/engine/primitive/surface.h>
#include <o3d/engine/material/ambientmaterial.h>
#include <o3d/engine/material/lambertmaterial.h>
#include <o3d/engine/material/pickingmaterial.h>

void MeshHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

//    o3d::Mesh *mesh = new o3d::Mesh(masterScene->scene());
//    mesh->setName(m_name);

    // Add a simple plane for simulate a ground to project shadow on
    Surface surface(1000, 1000, 4, 4);
    o3d::Mesh *mesh = new o3d::Mesh(masterScene->scene());
    mesh->setName(m_name);
    MeshData *meshData = new MeshData(mesh);

    GeometryData *surfaceGeometry = new GeometryData(meshData, surface);
    surfaceGeometry->genNormals();
    //surfaceGeometry->genTangentSpace();

    meshData->setGeometry(surfaceGeometry);
    meshData->computeBounding(GeometryData::BOUNDING_BOX);
    meshData->createGeometry();

    mesh->setMeshData(meshData);

    mesh->setNumMaterialProfiles(1);
    mesh->getMaterialProfile(0).setNumTechniques(1);
    mesh->getMaterialProfile(0).getTechnique(0).setNumPass(1);
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::AMBIENT, new AmbientMaterial(mesh));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::LIGHTING, new LambertMaterial(mesh));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::PICKING, new PickingMaterial(mesh));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::DEFERRED, new LambertMaterial(mesh));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setAmbient(Color(0.0f, 0.0f, 0.0f, 1.f));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setDiffuse(Color(1.0f, 1.0f, 1.0f, 1.f));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setSpecular(Color(0.0f, 0.0f, 0.0f, 1.f));
    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setShine(1.f);
    mesh->initMaterialProfiles();

    // if the parent hub is a spacial node add the mesh the it
    if (parent() && parent()->typeRef() == Application::instance()->types().typeRef("o3s::common::hub::spacialhub")) {
        SpacialNodeHub *parentHub = static_cast<SpacialNodeHub*>(parent());
        parentHub->addChildToScene(masterScene, mesh);
    }

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
        mesh->getParent()->deleteChild(mesh);

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

/**
 * @brief Common skin mesh component offering a 3d skinned mesh.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/skin.h>

#include "o3d/studio/common/component/skinhub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"
#include "o3d/studio/common/component/spacialnodehub.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

using namespace o3d::studio::common;


SkinComponent::SkinComponent()
{
    m_name = "o3s::common::component::skinhub";
    m_targetName = "o3s::common::hub::skinhub";

    m_label = "Skinned mesh Hub";
    m_icon = ":/icons/directions_run_black.svg";
}

SkinComponent::~SkinComponent()
{

}

void SkinComponent::setup()
{

}

Hub *SkinComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    SkinHub *skinHub = new SkinHub(name, parent);
    skinHub->setTypeRef(m_targetTypeRef);

    return skinHub;
}

SceneUIElement *SkinComponent::sceneUiElement(SceneUIElement::Mode mode)
{
    return nullptr;
}

Panel *SkinComponent::panel(Panel::PanelType panelType, Hub *hub)
{
    return nullptr;
}

SkinHub::SkinHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_instances()
{

}

SkinHub::~SkinHub()
{
    O3D_ASSERT(m_instances.empty());
}

void SkinHub::create()
{
    Hub::create();

    // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void SkinHub::destroy()
{
    Hub::destroy();

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool SkinHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool SkinHub::load()
{
    return Hub::load();
}

o3d::Bool SkinHub::save()
{
    return Hub::save();
}

o3d::Bool SkinHub::exists() const
{
    return Entity::exists();
}

o3d::Bool SkinHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool SkinHub::deserializeContent(InStream &stream)
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

#include <o3d/engine/texture/texturemanager.h>
#include <o3d/engine/texture/texture2d.h>

void SkinHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

//    o3d::Mesh *mesh = new o3d::Mesh(masterScene->scene());
//    mesh->setName(m_name);

//    MeshData *meshData = new MeshData(mesh);

//    if (m_vertices.isEmpty()) {
//        // Add a simple plane for simulate a ground to project shadow on
//        Surface surface(1000, 1000, 4, 4);

//        GeometryData *surfaceGeometry = new GeometryData(meshData, surface);
//        surfaceGeometry->genNormals();
//        //surfaceGeometry->genTangentSpace();

//        meshData->setGeometry(surfaceGeometry);
//    } else {
//        GeometryData *geometry = new GeometryData(meshData);

//        geometry->createElement(V_VERTICES_ARRAY, m_vertices);
//        geometry->createElement(V_NORMALS_ARRAY, m_normals);

//        if (m_uvs.isValid()) {
//            geometry->createElement(V_UV_MAP_ARRAY, m_uvs);
//        }

//        FaceArrayUInt32 *faceArray = new FaceArrayUInt32(masterScene->scene()->getContext());
//        faceArray->setFaces(m_indices);

//        geometry->addFaceArray(0, faceArray);

//        meshData->setGeometry(geometry);
//    }

//    meshData->computeBounding(GeometryData::BOUNDING_BOX);
//    meshData->createGeometry();

//    mesh->setMeshData(meshData);

//    mesh->setNumMaterialProfiles(1);
//    mesh->getMaterialProfile(0).setNumTechniques(1);
//    mesh->getMaterialProfile(0).getTechnique(0).setNumPass(1);
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::AMBIENT, new AmbientMaterial(mesh));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::LIGHTING, new LambertMaterial(mesh));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::PICKING, new PickingMaterial(mesh));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setMaterial(Material::DEFERRED, new LambertMaterial(mesh));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setAmbient(Color(1.0f, 1.0f, 1.0f, 1.f));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setDiffuse(Color(1.0f, 1.0f, 1.0f, 1.f));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setSpecular(Color(0.0f, 0.0f, 0.0f, 1.f));
//    mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setShine(1.f);
//    mesh->initMaterialProfiles();

//    // @todo material hub and texture resource
//    // @todo as ambient to because no light for the moment
//    if (m_diffuseMap.isValid()) {
//        Texture2D *diffuseMap = masterScene->scene()->getTextureManager()->addTexture2D(m_diffuseMap, True);
//        mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setDiffuseMap(diffuseMap);
//        mesh->getMaterialProfile(0).getTechnique(0).getPass(0).setAmbientMap(diffuseMap);
//    }

//    // if the parent hub is a spacial node add the mesh the it
//    if (parent() && parent()->typeRef() == Application::instance()->types().typeRef("o3s::common::hub::spacialhub")) {
//        SpacialNodeHub *parentHub = static_cast<SpacialNodeHub*>(parent());
//        parentHub->addChildToScene(masterScene, mesh);
//    }

//    m_instances[masterScene] = mesh;

    // scene object id is as the base of the pickable color id
//    project()->addPickable((UInt32)skin->getId(), this);

    O3D_MESSAGE("SkinHub created into scene");
}

void SkinHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Skin *skin = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)skin->getId());

        skin->getParent()->deleteChild(skin);

        O3D_MESSAGE("SkinHub deleted from scene");
    }
}

void SkinHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Skin *skin = it->second;

        // hub => o3d
        skin->setName(m_name);

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("SkinHub synced into scene");
    }
}

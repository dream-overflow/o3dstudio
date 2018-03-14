/**
 * @brief Common camera component offering a camera hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>

#include "o3d/studio/common/component/camerahub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


CameraHubComponent::CameraHubComponent()
{
    m_name = "o3s::common::component::camerahub";
    m_targetName = "o3s::common::hub::camera";

    m_label = "Camera Hub";
    m_icon = ":/icons/videocam_black.svg";
}

CameraHubComponent::~CameraHubComponent()
{

}

void CameraHubComponent::setup()
{

}

Hub *CameraHubComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    CameraHub *cameraHub = new CameraHub(name, parent);
    cameraHub->setTypeRef(m_targetTypeRef);

    return cameraHub;
}

CameraHub::CameraHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_left(0.f),
    m_right(640.f),
    m_bottom(480.f),
    m_top(0.f),
    m_coef(1.33333f),
    m_fov(60.f),
    m_znear(0.5f),
    m_zfar(1000.0f),
    m_cameraMode(CAMERA_PERSPECTIVE),
    m_instances()
{

}

CameraHub::~CameraHub()
{
    O3D_ASSERT(m_instances.empty());
}

void CameraHub::create()
{
    Hub::create();

    // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void CameraHub::destroy()
{
    Hub::destroy();

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool CameraHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool CameraHub::load()
{
    return Hub::load();
}

o3d::Bool CameraHub::save()
{
    return Hub::save();
}

o3d::Bool CameraHub::exists() const
{
    return Entity::exists();
}

o3d::Bool CameraHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool CameraHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void CameraHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Camera *camera = new o3d::Camera(masterScene->scene());
    camera->setName(m_name);

    camera->setLeft(m_left);
    camera->setRatio(m_right);
    camera->setBottom(m_bottom);
    camera->setTop(m_top);

    camera->setZnear(m_znear);
    camera->setZfar(m_zfar);

    camera->setRatio(m_coef);
    camera->setFov(m_fov);

    if (m_cameraMode == CAMERA_ORTHO_BY_FOV) {
        camera->computeOrthoByFov();
    } else if (m_cameraMode == CAMERA_ORTHO) {
        camera->computeOrtho();
    } else if (m_cameraMode == CAMERA_PERSPECTIVE) {
        camera->computePerspective();
    }

    m_instances[masterScene] = camera;

    // scene object id is as the base of the pickable color id
    project()->addPickable((UInt32)camera->getId(), this);

    O3D_MESSAGE("CameraHub created into scene");
}

void CameraHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Camera *camera = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)camera->getId());

        camera->getParent()->deleteChild(camera);

        O3D_MESSAGE("CameraHub deleted from scene");
    }
}

void CameraHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Camera *camera = it->second;

        // hub => o3d
        camera->setName(m_name);

        camera->setLeft(m_left);
        camera->setRatio(m_right);
        camera->setBottom(m_bottom);
        camera->setTop(m_top);

        camera->setZnear(m_znear);
        camera->setZfar(m_zfar);

        camera->setRatio(m_coef);
        camera->setFov(m_fov);

        if (m_cameraMode == CAMERA_ORTHO_BY_FOV) {
            camera->computeOrthoByFov();
        } else if (m_cameraMode == CAMERA_ORTHO) {
            camera->computeOrtho();
        } else if (m_cameraMode == CAMERA_PERSPECTIVE) {
            camera->computePerspective();
        }

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("CameraHub synced into scene");
    }
}

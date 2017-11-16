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

using namespace o3d::studio::common;


CameraComponent::CameraComponent()
{
    m_name = "o3s::common::component::camerahub";
    m_targetName = "o3s::common::hub::camera";
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::setup()
{

}

Hub *CameraComponent::buildHub(const QString &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    CameraHub *cameraHub = new CameraHub(name, parent);
    cameraHub->setProject(project);
    cameraHub->setTypeRef(m_targetTypeRef);

    if (parent->ref().light().baseTypeOf(TypeRef::project())) {
        project->addHub(cameraHub);
    } else if (parent->ref().light().baseTypeOf(TypeRef::hub())) {
        Hub *parentHub = static_cast<Hub*>(parent);
        parentHub->addHub(cameraHub);
    } else {
        delete cameraHub;
        return nullptr;
    }

    return cameraHub;
}

CameraHub::CameraHub(const QString &name, Entity *parent) :
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
    // @todo it is not the solution, we need a propre method of deallocation of a hub
    // and it need to be per instance, but it also need for somes to setup the context before
    // so we need a sort of pull list to be done during sync when the renderer is activated for
    // example
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        o3d::Camera *camera = it.value();
        delete camera;
    }
}

void CameraHub::create()
{

}

bool CameraHub::load()
{
    return Hub::load();
}

bool CameraHub::save()
{
    return Hub::save();
}

bool CameraHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

bool CameraHub::serializeContent(QDataStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return false;
    }

    return true;
}

bool CameraHub::deserializeContent(QDataStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return false;
    }

    return true;
}

void CameraHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Camera *camera = new o3d::Camera(masterScene->scene());

    camera->setName(m_name.toStdWString().c_str());

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

    m_instances.insert(masterScene, camera);
}

void CameraHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Camera *camera = it.value();
        m_instances.erase(it);
        delete camera;
    }
}

void CameraHub::syncWithScene(MasterScene *masterScene)
{
    // @todo how to
}

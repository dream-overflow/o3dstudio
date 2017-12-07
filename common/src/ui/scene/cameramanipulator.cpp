/**
 * @brief Common UI scene camera manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @details
 */

#include "o3d/studio/common/ui/scene/cameramanipulator.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/object/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


CameraManipulator::CameraManipulator(BaseObject *parent, const Point2f &pos, const Float scale) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_pos(pos),
    m_scale(scale)
{

}

CameraManipulator::~CameraManipulator()
{

}

void CameraManipulator::createToScene(MasterScene *)
{

}

void CameraManipulator::removeFromScene(MasterScene *)
{

}

void CameraManipulator::syncWithScene(MasterScene *)
{

}

void CameraManipulator::directRendering(Scene *scene)
{
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    // Get ratio from active scene camera (could do it from primary viewport)
    Float ratio = scene->getActiveCamera()->getRatio();

    scene->getContext()->enableDepthTest();
    scene->getContext()->enableDepthWrite();
    scene->getContext()->setDefaultDepthTest();
    scene->getContext()->setDefaultDepthFunc();

    // setup modelview
    Matrix4 mv;
    mv.setRotation(scene->getActiveCamera()->getModelviewMatrix().getRotation());
    mv.setTranslation(0.4f*ratio, 0.4f, 0.f);
    primitive->modelView().set(mv);

    // and project to ortho
    Matrix4 pj;
    pj.buildOrtho(-0.5f*ratio, 0.5f*ratio, -0.5f, 0.5f, -1.f, 1.f);
    primitive->projection().set(pj);

//    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_HINT_NICEST);
    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);

    scene->getContext()->setCullingMode(CULLING_BACK_FACE);
    scene->getContext()->disableDoubleSide();
    scene->getContext()->enableDepthTest();

    primitive->modelView().push();
    primitive->setColor(0.6f, 0.6f, 0.6f);

    // 5% of the scale
    Vector3 scale(m_scale*0.05f, m_scale*0.05f, m_scale*0.05f);

    // global cube
    primitive->draw(PrimitiveManager::SOLID_CUBE1, scale);

    // X
    scale.set(m_scale*0.02f, m_scale*0.05f, m_scale*0.02f);
    primitive->setColor(0.8f, 0.0f, 0.0f);

    // X+ cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(m_scale*0.060f, 0, 0));
    primitive->modelView().rotateZ(o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    // X- cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(-m_scale*0.060f, 0, 0));
    primitive->modelView().rotateZ(-o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    // Y
    scale.set(m_scale*0.02f, m_scale*0.05f, m_scale*0.02f);
    primitive->setColor(0.f, 0.8f, 0.0f);

    // Y+ cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(0, m_scale*0.060f, 0));
    primitive->modelView().rotateX(o3d::PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    // Y- cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(0, -m_scale*0.060f, 0));
    // primitive->modelView().rotateX(0);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    // Z
    scale.set(m_scale*0.02f, m_scale*0.05f, m_scale*0.02f);
    primitive->setColor(0.0f, 0.0f, 0.8f);

    // Z+ cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(0, 0, m_scale*0.060f));
    primitive->modelView().rotateX(-o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    // Z- cone
    primitive->modelView().push();
    primitive->modelView().translate(Vector3(0, 0, -m_scale*0.060f));
    primitive->modelView().rotateX(o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, scale);
    primitive->modelView().pop();

    primitive->modelView().pop();

    // restore
    scene->getContext()->setAntiAliasing(aa);
}

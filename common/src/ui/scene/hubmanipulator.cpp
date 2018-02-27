/**
 * @brief Common UI scene hub manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-24
 * @details
 */

#include "o3d/studio/common/ui/scene/hubmanipulator.h"
#include "o3d/studio/common/workspace/hub.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/drawinfo.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


HubManipulator::HubManipulator(BaseObject *parent, Hub* target, const Matrix4 &transform) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_transform(transform),
    m_scale(1),
    m_pickingMask(0xffffffff),  // @todo picking mask range and mask generator
    m_axe(-1),
    m_delta(0)
{
    m_targets.push_back(target);
}

HubManipulator::HubManipulator(
        BaseObject *parent,
        std::list<Hub *> targets,
        const Matrix4 &transform) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_targets(targets),
    m_transform(transform),
    m_scale(1),
    m_axe(-1),
    m_delta(0)
{

}

HubManipulator::~HubManipulator()
{

}

void HubManipulator::setup(MasterScene *masterScene)
{
    // register the picking colors (@todo by range)
    masterScene->registerPickingId(0xffff00ff, this);
    masterScene->registerPickingId(0xffff01ff, this);
    masterScene->registerPickingId(0xffff02ff, this);
    masterScene->registerPickingId(0xffff03ff, this);
}

void HubManipulator::release(MasterScene *masterScene)
{
    // register the picking colors (@todo by range)
    masterScene->unregisterPickingId(0xffff00ff);
    masterScene->unregisterPickingId(0xffff01ff);
    masterScene->unregisterPickingId(0xffff02ff);
    masterScene->unregisterPickingId(0xffff03ff);
}

void HubManipulator::hover(o3d::UInt32 id, const o3d::Point3f &pos)
{
    // @todo according to the id mask
    if (id == 0xffff00ff) {

    } else if (id == 0xffff01ff) {

    } else if (id == 0xffff02ff) {

    } else if (id == 0xffff03ff) {

    }
}

void HubManipulator::leave()
{
    m_axe = AXE_NONE;
    m_delta = 0;
}

void HubManipulator::createToScene(MasterScene *)
{

}

void HubManipulator::removeFromScene(MasterScene *)
{

}

void HubManipulator::syncWithScene(MasterScene *)
{

}

void HubManipulator::directRendering(DrawInfo &drawInfo, MasterScene *masterScene)
{
    Scene *scene = masterScene->scene();

    if (drawInfo.pass != DrawInfo::PICKING_PASS && drawInfo.pass != DrawInfo::AMBIENT_PASS) {
        return;
    }

    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();
    Context &context = *scene->getContext();

    const Box2i &vp = context.getViewPort();

    // biaised depth
    context.enableDepthTest();
    context.enableDepthWrite();
    context.setDepthRange(0.0, 0.1);

    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);  // AA_HINT_NICEST

    // computed visibility and 2d mapped position
    Matrix4 mv;

    Vector3 v = Matrix::projectPoint(
                    scene->getActiveCamera()->getProjectionMatrix(),
                    scene->getActiveCamera()->getModelviewMatrix() * m_transform,
                    vp,
                    Vector3());

    // outside
    if (v.z() < 0) {
        return;
    }

    //        // pos and rotate from the transform
    //        // mv.setRotation(...);
    //        mv.setTranslation(v.x(), v.y(), 0.f);

    //        primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * mv);

    //        // and project to ortho
    //        Matrix4 pj;
    //        pj.buildOrtho(vp.x(), vp.x2(), vp.y(), vp.y2(), m_scale * -(factor*0.1f), m_scale * factor*0.1f);
    //        primitive->projection().set(pj);

    Float s = m_scale * (scene->getActiveCamera()->getAbsoluteMatrix().getTranslation() - m_transform.getTranslation()).length() * 0.1;

    scene->getActiveCamera()->setProjectionMatrix();
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * m_transform);

    primitive->modelView().push();
    primitive->setColor(1, 1, 1);

    if (drawInfo.pass == DrawInfo::PICKING_PASS) {
        // a picking pass for manipulation
        if (masterScene->actionMode() == MasterScene::ACTION_ROTATION) {
            // @todo
        } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
            // translation axes as cylinder
            primitive->modelView().push();
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(Color(255, 255, 0, 255));   // x picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*0.8f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(Color(255, 255, 1, 255));   // y picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*0.8f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(Color(255, 255, 2, 255));   // z picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*0.8f,s*0.1f));
            primitive->modelView().pop();

            // @todo a planar transformation helper
        } else if (masterScene->actionMode() == MasterScene::ACTION_SCALE) {
            // @todo
        }
    } else if (drawInfo.pass == DrawInfo::AMBIENT_PASS) {
        // @todo an highlighted bounding volume ??

        // an highlighted version of the object
        // @todo

        if (masterScene->actionMode() == MasterScene::ACTION_ROTATION) {
            // rotation circles
            // @todo 0.5a circle in area of current angle

            // translation axes
            primitive->setModelviewProjection();
            primitive->drawXYZAxis(Vector3(s, s, s));

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(s*0.8f, 0, 0));
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(1.f,0.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(0.f,1.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(0.f,0.f,1.f);
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();
        } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
            // translation axes
            primitive->setModelviewProjection();
            primitive->drawXYZAxis(Vector3(s, s, s));

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(s*0.8f, 0, 0));
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(1.f,0.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_CONE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(0.f,1.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_CONE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(0.f,0.f,1.f);
            primitive->draw(PrimitiveManager::SOLID_CONE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();
        } else if (masterScene->actionMode() == MasterScene::ACTION_SCALE) {
            // scale axes with squares
            // @todo 0.5a square in direction and proportional to current scale

            // translation axes
            primitive->setModelviewProjection();
            primitive->drawXYZAxis(Vector3(s, s, s));

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(s*0.8f, 0, 0));
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(1.f,0.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(0.f,1.f,0.f);
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(0.f,0.f,1.f);
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();
        }
    }

    primitive->modelView().pop();

    // restore
    context.setDefaultDepthWrite();
    context.setDefaultDepthTest();
    context.setDefaultDepthRange();
    context.setAntiAliasing(aa);
}

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
    masterScene->registerPickingId(0xffffff00, this);
    masterScene->registerPickingId(0xffffff01, this);
    masterScene->registerPickingId(0xffffff02, this);
    masterScene->registerPickingId(0xffffff03, this);
}

void HubManipulator::release(MasterScene *masterScene)
{
    // register the picking colors (@todo by range)
    masterScene->unregisterPickingId(0xffffff00);
    masterScene->unregisterPickingId(0xffffff01);
    masterScene->unregisterPickingId(0xffffff02);
    masterScene->unregisterPickingId(0xffffff03);
}

void HubManipulator::hover(o3d::UInt32 id, const o3d::Point3f &pos)
{
    // @todo according to the id mask
    if (id == 0xffffff00) {
        m_axe = AXE_X;
    } else if (id == 0xffffff01) {
        m_axe = AXE_Y;
    } else if (id == 0xffffff02) {
        m_axe = AXE_Z;
    } else if (id == 0xffffff03) {
        m_axe = AXE_MANY;
    } else {
        m_axe = AXE_NONE;
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

    PrimitiveAccess primitive = scene->getPrimitiveManager()->access(drawInfo);
    Context &context = *scene->getContext();

    const Box2i &vp = context.getViewPort();

    // biaised depth
    context.enableDepthTest();
    context.enableDepthWrite();
    context.setDepthRange(0.0, 0.1);

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
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * m_transform);
    primitive->modelView().push();

    if (drawInfo.pass == DrawInfo::PICKING_PASS) {
        // assume picking project matrix is always current

        // a picking pass for manipulation
        if (masterScene->actionMode() == MasterScene::ACTION_ROTATION) {
            // translation axes as cylinder
            primitive->modelView().push();
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setPickableId(0xffffff00);   // x picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->setPickableId(0xffffff01);   // y picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setPickableId(0xffffff02);   // z picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            // @todo
        } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
            // translation axes as cylinder
            primitive->modelView().push();
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setPickableId(0xffffff00);   // x picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->setPickableId(0xffffff01);   // y picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setPickableId(0xffffff02);   // z picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            // @todo a planar transformation helper
        } else if (masterScene->actionMode() == MasterScene::ACTION_SCALE) {
            // translation axes as cylinder
            primitive->modelView().push();
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setPickableId(0xffffff00);   // x picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->setPickableId(0xffffff01);   // y picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*1.f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setPickableId(0xffffff02);   // z picking id
            primitive->draw(PrimitiveManager::SOLID_CYLINDER1, Vector3(s*0.1f,s*0.8f,s*0.1f));
            primitive->modelView().pop();

            // @todo
        }
    } else if (drawInfo.pass == DrawInfo::AMBIENT_PASS) {
        Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);  // AA_HINT_NICEST

        // active camera project matrix
        scene->getActiveCamera()->setProjectionMatrix();

        primitive->setColor(1, 1, 1);

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
            primitive->setColor(axeColor(AXE_X));
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(axeColor(AXE_Y));
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(axeColor(AXE_Z));
            primitive->draw(PrimitiveManager::SOLID_SPHERE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();
        } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
            // translation axes
            primitive->setModelviewProjection();
            primitive->drawXYZAxis(Vector3(s, s, s));

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(s*0.8f, 0, 0));
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(axeColor(AXE_X));
            primitive->draw(PrimitiveManager::SOLID_CONE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(axeColor(AXE_Y));
            primitive->draw(PrimitiveManager::SOLID_CONE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(axeColor(AXE_Z));
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
            primitive->setColor(axeColor(AXE_X));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.8f,0));
            primitive->setColor(axeColor(AXE_Y));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.8f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(axeColor(AXE_Z));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.1f,s*0.2f,s*0.1f));
            primitive->modelView().pop();
        }

        context.setAntiAliasing(aa);
    }

    primitive->modelView().pop();

    // restore
    context.setDefaultDepthWrite();
    context.setDefaultDepthTest();
    context.setDefaultDepthRange();
}

o3d::Color HubManipulator::axeColor(HubManipulator::Axe axe)
{
    if (m_axe == AXE_MANY) {
        // @todo depends...
    } else if (m_axe == axe) {
        if (axe == AXE_X) {
            return Color(1.f, 0.5f, 0.5f);
        }else if (axe == AXE_Y) {
            return Color(0.5f, 1.0f, 0.5f);
        } else if (axe == AXE_Z) {
            return Color(0.5f, 0.5f, 1.f);
        }
    } else {
        if (axe == AXE_X) {
            return Color(1.f, 0.f, 0.f);
        } else if (axe == AXE_Y) {
            return Color(0.f, 1.f, 0.f);
        } else if (axe == AXE_Z) {
           return Color(0.f, 0.f, 1.f);
        }
    }

    return Color();
}

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
    m_scale(1)
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
    m_scale(1)
{

}

HubManipulator::~HubManipulator()
{

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

    if (drawInfo.pass == DrawInfo::PICKING_PASS) {
        // @todo a picking pass for manipulation
    } else if (drawInfo.pass == DrawInfo::AMBIENT_PASS) {
        PrimitiveAccess primitive = scene->getPrimitiveManager()->access();
        Context &context = *scene->getContext();

        const Box2i &vp = context.getViewPort();
        const Float factor = 600.f;

        context.enableDepthTest();
        context.enableDepthWrite();
        context.setDefaultDepthTest();
        context.setDefaultDepthFunc();

        Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);  // AA_HINT_NICEST

        // setup modelview
        Matrix4 mv;

        Vector3 v = Matrix::projectPoint(
                    scene->getActiveCamera()->getProjectionMatrix(),
                    scene->getActiveCamera()->getModelviewMatrix(),// * m_transform,
                    vp,
                    /*Vector3() */m_transform.getTranslation());

        // outside
        if (v.z() < 0) {
            return;
        }

        // @todo and rotate from the transform
        //mv.setRotation(scene->getActiveCamera()->getModelviewMatrix().getRotation());
        //mv.setTranslation(v.x(), v.y(), 0.f);

        Quaternion q;
        q.fromMatrix3(scene->getActiveCamera()->getModelviewMatrix().getRotation());
        q.normalize();

        Vector3 rot;
        q.toEuler(rot);

        q.fromEuler(Vector3(rot.x(), rot.y(), rot.z()));
        q.normalize();
        // q.invert();
       // mv.setRotation(q.toMatrix3() * m_transform.getRotation());

        Matrix4 mmv;
        //mv = q.toMatrix4();
        mv.setTranslation(v.x(), v.y(), 0.f);
        mmv.setRotation(scene->getActiveCamera()->getAbsoluteMatrix().getRotation().invert());

        // mv.setRotation(scene->getActiveCamera()->getAbsoluteMatrix().getRotation().invert() * m_transform.getRotation());
        primitive->modelView().set(mv * mmv);

        // and project to ortho
        Matrix4 pj;
        pj.buildOrtho(vp.x(), vp.x2(), vp.y(), vp.y2(), m_scale * -(factor*0.1f), m_scale * factor*0.1f);
        primitive->projection().set(pj);

        Float s = 75 * m_scale;

//        // no depth
//        context.disableDepthWrite();
//        context.disableDepthTest();

        // scene->getActiveCamera()->setProjectionMatrix();

        primitive->modelView().push();

//        // compute scale to have a constant size
//        Float scale = (scene->getActiveCamera()->getModelviewMatrix().getTranslation() - m_transform.getTranslation()).length();

//        Matrix4 mv = m_transform;
//        mv.scale(scale);

//        primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * mv);

        primitive->setColor(1, 1, 1);

//        // @todo an highlighted bounding volume ??

//        // an highlighted version of the object
//        // @todo

        if (masterScene->actionMode() == MasterScene::ACTION_ROTATION) {
            // rotation ball
            // @todo
        } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
            // translation axes
            // @todo
            primitive->drawXYZAxis(Vector3(10, 10, 10));
        } else if (masterScene->actionMode() == MasterScene::ACTION_SCALE) {
            // scale axes
            // @todo
        } else {
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
        }

        primitive->modelView().pop();

        // restore
        context.setDefaultDepthWrite();
        context.setDefaultDepthTest();
        context.setAntiAliasing(aa);
    }
}

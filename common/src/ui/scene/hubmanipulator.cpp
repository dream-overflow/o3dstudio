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
#include <o3d/engine/object/transform.h>

#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


HubManipulator::HubManipulator(BaseObject *parent, Hub* target) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_transform(),
    m_scale(1),
    m_pickingMask(0xfffffff0f),  // @todo picking mask range and mask generator
    m_axe(AXE_NONE),
    m_transformMode(STATIC)
{
    m_targets.push_back(target);
}

HubManipulator::HubManipulator(
        BaseObject *parent,
        std::list<Hub *> targets,
        const Matrix4 &transform) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_targets(targets),
    m_transform(),
    m_scale(1),
    m_pickingMask(0xfffffff0f),  // @todo
    m_axe(AXE_NONE),
    m_transformMode(STATIC)
{

}

HubManipulator::~HubManipulator()
{

}

void HubManipulator::setup(MasterScene *masterScene)
{
    updateTransform(masterScene);

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
    if (m_focus) {
        return;
    }

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
    if (m_focus) {
        return;
    }

    m_axe = AXE_NONE;
}

void HubManipulator::beginTransform(MasterScene *masterScene)
{
    O3D_ASSERT(m_orgV.empty());
    O3D_ASSERT(m_orgQ.empty());

    m_focus = True;
    m_delta.zero();

    updateTransform(masterScene);

    if (masterScene->actionMode() == MasterScene::ACTION_ROTATION) {
        m_transformMode = ROTATE;
    } else if (masterScene->actionMode() == MasterScene::ACTION_TRANSLATION) {
        m_transformMode = TRANSLATE;

        for (Hub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub);
                m_orgV.push_back(spacialNode->transform(0).getPosition());
            } else if (hub->isParentSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                m_orgV.push_back(spacialNode->transform(0).getPosition());
            } else {
                m_orgV.push_back(Vector3());
            }
        }
    } else if (masterScene->actionMode() == MasterScene::ACTION_SCALE) {
        m_transformMode = SCALE;
    // }  else if (masterScene->actionMode() == MasterScene::ACTION_SKEW) {
    //    m_transform = SKEW;
    } else {
        m_transformMode = STATIC;
    }
}

void HubManipulator::transform(const o3d::Vector3f &v, MasterScene *masterScene)
{
    Float s = 1.f;

    if (masterScene->speedModifier() == MasterScene::SPEED_FAST) {
        s *= 10.f;
    } else if (masterScene->speedModifier() == MasterScene::SPEED_SLOW) {
        s *= 0.1f;
    }

    if (m_transformMode == ROTATE) {
        // @todo
    } else if (m_transformMode == TRANSLATE) {
        if (m_axe == AXE_X) {
            m_delta.x() += v.x() * s;
        } else if (m_axe == AXE_Y) {
            m_delta.y() += v.y() * s;
        } else if (m_axe == AXE_Z) {
            m_delta.z() += v.z() * s;
        } else if (m_axe == AXE_MANY) {
            m_delta.x() += v.x() * s;
            m_delta.y() += v.y() * s;
        }

        auto it = m_orgV.begin();
        SpacialNodeHub *spacialNode;

        for (Hub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
            } else {
                spacialNode = nullptr;
            }

            if (spacialNode) {
                spacialNode->setPosition(0, (*it) + m_delta);

                SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                masterScene->addCommand(sceneCommand);
            }

            ++it;
        }

        m_position = m_delta;
    } else if (m_transformMode == SCALE) {

    // }  else if (m_transformMode == SKEW) {

    }
}

void HubManipulator::endTransform()
{
    m_focus = False;
    m_transformMode = STATIC;
    m_delta.zero();

    m_orgQ.clear();
    m_orgV.clear();
}

o3d::Bool HubManipulator::isTransform() const
{
    return m_transformMode != STATIC;
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

    // transform by current delta
    Matrix4 local = m_transform;
    local.translate(m_position);
    // local *= m_rotation.toMatrix4();

    // computed visibility and 2d mapped position
    Matrix4 mv;

    Vector3 v = Matrix::projectPoint(
                    scene->getActiveCamera()->getProjectionMatrix(),
                    scene->getActiveCamera()->getModelviewMatrix() * local,
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

    Float s = m_scale * (scene->getActiveCamera()->getAbsoluteMatrix().getTranslation() - local.getTranslation()).length() * 0.1;
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * local);
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

void HubManipulator::updateTransform(MasterScene *masterScene)
{
    // synchro with hubs transformation
    Vector3 pos;
    Quaternion rot;
    UInt32 c = 0;

    for (Hub* hub : m_targets) {
        if (hub->isSpacialNode() || hub->isParentSpacialNode()) {
            const Matrix4 &m = hub->absoluteMatrix(masterScene);

            pos += m.getTranslation();
            rot += Quaternion(m.getRotation());

            ++c;
        }
    }

    if (c) {
        pos *= 1.f / c;
        rot *= 1.f / c;

        m_transform.identity();
        m_transform.setTranslation(pos);
        m_transform.setRotation(rot.toMatrix3());
    }

    // reset relative transformation
    m_position.zero();
    m_rotation.identity();
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

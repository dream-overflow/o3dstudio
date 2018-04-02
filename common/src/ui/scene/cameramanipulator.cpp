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
#include <o3d/engine/picking.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/hierarchy/node.h>
#include <o3d/engine/drawinfo.h>
#include <o3d/engine/object/stransform.h>
#include <o3d/engine/object/transform.h>
#include <o3d/engine/hierarchy/node.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


CameraManipulator::CameraManipulator(BaseObject *parent, const Point2f &pos, const Float scale) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_pos(pos),
    m_scale(scale),
    m_hoverPart(PART_NONE),
    m_activePart(PART_NONE),
    m_action(ACTION_NONE)
{

}

CameraManipulator::~CameraManipulator()
{

}

void CameraManipulator::setup(MasterScene *masterScene)
{
    // register the picking colors (cube + 6 sides)
    for (int i = PART_CUBE; i <= PART_Z_MINUS; ++i) {
        masterScene->registerPickingId(i, this);
    }
}

void CameraManipulator::release(MasterScene *masterScene)
{
    // unregister the picking colors
    for (int i = PART_CUBE; i <= PART_Z_MINUS; ++i) {
        masterScene->unregisterPickingId(i);
    }
}

void CameraManipulator::hover(o3d::UInt32 id, const o3d::Point3f &pos)
{
    m_hoverPart = (Part)id;
}

void CameraManipulator::leave()
{
    m_hoverPart = PART_NONE;
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

void CameraManipulator::directRendering(DrawInfo &drawInfo, MasterScene *masterScene)
{
    if (drawInfo.pass != DrawInfo::AMBIENT_PASS && drawInfo.pass != DrawInfo::PICKING_PASS) {
        return;
    }

    Scene *scene = masterScene->scene();
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access(drawInfo);

    // const Box2i &vp = scene->getContext()->getViewPort();
    const Box2i &vp = masterScene->viewPort();
    const Float factor = 600.f;

    // Get ratio from active scene camera (could do it from primary viewport)
    // Float ratio = scene->getActiveCamera()->getRatio();

    scene->getContext()->enableDepthTest();
    scene->getContext()->enableDepthWrite();
    scene->getContext()->setDefaultDepthTest();
    scene->getContext()->setDefaultDepthFunc();

    // setup modelview
    Matrix4 mv;

    STransform *tr = static_cast<STransform*>(scene->getActiveCamera()->getNode()->getTransform());

    Quaternion q1, q2;
    q1.fromEuler(Vector3(-tr->getEuler().x(), -tr->getEuler().y(), 0));
    q2.fromAxisAngle3(Vector3(0, 0, 1), -tr->getEuler().z());

    q1 *= q2;

    mv.setRotation(q1.toMatrix3().invert());

    // mv.setTranslation(480 * m_scale, 290 * m_scale, -600 * m_scale); // approx in perspective mode
    mv.setTranslation(vp.x2() - m_scale * factor*0.1f, vp.y2() - m_scale * factor*0.1f, 0.f);
    primitive->modelView().set(mv);

    // and project to ortho
    Matrix4 pj;
    Matrix4 oldPj = scene->getContext()->projection().get();
    pj.buildOrtho(vp.x(), vp.x2(), vp.y(), vp.y2(), m_scale * -(factor*0.1f), m_scale * factor*0.1f);

    Context::AntiAliasingMethod aa;

    if (drawInfo.pass == DrawInfo::PICKING_PASS) {
        Vector2f pos = scene->getPicking()->getWindowPos();
        Vector2f size = scene->getPicking()->getWindowSize();

        // create size pixel picking region near cursor location
        Matrix4 regionMatrix = ProjectionMatrix::pickMatrix(pos, size, vp);
        primitive->projection().set(regionMatrix);
        primitive->projection().mult(pj);
    } else {
        aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);  // AA_HINT_NICEST

        primitive->projection().set(pj);
    }

    scene->getContext()->setCullingMode(CULLING_BACK_FACE);
    scene->getContext()->disableDoubleSide();
    scene->getContext()->enableDepthTest();

    primitive->modelView().push();

    Float xs = m_scale*factor, ys = m_scale*factor, zs = m_scale*factor;

    // 5% of the scale
    Vector3 vscale(xs*0.05f, ys*0.05f, zs*0.05f);

    // global cube
    primitive->setPickableId(PART_CUBE);
    if (m_hoverPart == PART_CUBE || m_activePart == PART_CUBE) {
        primitive->setColor(0.8f, 0.8f, 0.8f);
    } else {
        primitive->setColor(0.6f, 0.6f, 0.6f);
    }
    primitive->draw(PrimitiveManager::SOLID_CUBE1, vscale);

    // X
    vscale.set(xs*0.02f, ys*0.05f, zs*0.02f);

    // X+ cone
    primitive->modelView().push();
    primitive->setPickableId(PART_X_PLUS);
    if (m_hoverPart == PART_X_PLUS || m_activePart == PART_X_PLUS) {
        primitive->setColor(1.0f, 0.5f, 0.5f);
    } else {
        primitive->setColor(0.8f, 0.0f, 0.0f);
    }
    primitive->modelView().translate(Vector3(xs*0.060f, 0, 0));
    primitive->modelView().rotateZ(o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    // X- cone
    primitive->modelView().push();
    primitive->setPickableId(PART_X_MINUS);
    if (m_hoverPart == PART_X_MINUS || m_activePart == PART_X_MINUS) {
        primitive->setColor(1.0f, 0.5f, 0.5f);
    } else {
        primitive->setColor(0.8f, 0.0f, 0.0f);
    }
    primitive->modelView().translate(Vector3(-xs*0.060f, 0, 0));
    primitive->modelView().rotateZ(-o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    // Y
    vscale.set(xs*0.02f, ys*0.05f, zs*0.02f);

    // Y+ cone
    primitive->modelView().push();
    primitive->setPickableId(PART_Y_PLUS);
    if (m_hoverPart == PART_Y_PLUS || m_activePart == PART_Y_PLUS) {
        primitive->setColor(0.5f, 1.0f, 0.5f);
    } else {
        primitive->setColor(0.0f, 0.6f, 0.0f);
    }
    primitive->modelView().translate(Vector3(0, ys*0.060f, 0));
    primitive->modelView().rotateX(o3d::PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    // Y- cone
    primitive->modelView().push();
    primitive->setPickableId(PART_Y_MINUS);
    if (m_hoverPart == PART_Y_MINUS || m_activePart == PART_Y_MINUS) {
        primitive->setColor(0.5f, 1.0f, 0.5f);
    } else {
        primitive->setColor(0.0f, 0.6f, 0.0f);
    }
    primitive->modelView().translate(Vector3(0, -ys*0.060f, 0));
    // primitive->modelView().rotateX(0);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    // Z
    vscale.set(xs*0.02f, ys*0.05f, zs*0.02f);
    primitive->setColor(0.0f, 0.0f, 0.8f);

    // Z+ cone
    primitive->modelView().push();
    primitive->setPickableId(PART_Z_PLUS);
    if (m_hoverPart == PART_Z_PLUS || m_activePart == PART_Z_PLUS) {
        primitive->setColor(0.5f, 0.5f, 1.0f);
    } else {
        primitive->setColor(0.0f, 0.0f, 0.8f);
    }
    primitive->modelView().translate(Vector3(0, 0, zs*0.060f));
    primitive->modelView().rotateX(-o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    // Z- cone
    primitive->modelView().push();
    primitive->setPickableId(PART_Z_MINUS);
    if (m_hoverPart == PART_Z_MINUS || m_activePart == PART_Z_MINUS) {
        primitive->setColor(0.5f, 0.5f, 1.0f);
    } else {
        primitive->setColor(0.0f, 0.0f, 0.8f);
    }
    primitive->modelView().translate(Vector3(0, 0, -zs*0.060f));
    primitive->modelView().rotateX(o3d::HALF_PI);
    primitive->draw(PrimitiveManager::SOLID_CONE1, vscale);
    primitive->modelView().pop();

    primitive->modelView().pop();

    scene->getContext()->disableDepthTest();

    // A simple axis to distinguis plus from minus direction
    primitive->setColor(1, 1, 1);
    primitive->setPickableId(PART_CUBE);
    primitive->setModelviewProjection();
    primitive->drawXYZAxis(Vector3(m_scale*15, m_scale*15, m_scale*15));

    // restore
    scene->getContext()->setDefaultDepthTest();
    scene->getContext()->projection().set(oldPj);

    if (drawInfo.pass == DrawInfo::AMBIENT_PASS) {
        scene->getContext()->setAntiAliasing(aa);
    }
}

o3d::Bool CameraManipulator::keyPressEvent(const KeyEvent &event, MasterScene *masterScene)
{
    return False;
}

o3d::Bool CameraManipulator::keyReleaseEvent(const KeyEvent &event, MasterScene *masterScene)
{
    return False;
}

o3d::Bool CameraManipulator::mousePressEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (event.button(Mouse::LEFT)) {
        if (masterScene->actionMode() == MasterScene::ACTION_NONE) {
            if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
                masterScene->setActionMode(MasterScene::ACTION_TRANSFORM);
                m_action = ACTION_TRANSLATION;

                // default normal speed, press again shift to slow
                masterScene->setMotionType(MasterScene::MOTION_FOLLOW);

                return True;
            } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
                masterScene->setActionMode(MasterScene::ACTION_TRANSFORM);
                m_action = ACTION_ZOOM;

                return True;
            } else {
                masterScene->setActionMode(MasterScene::ACTION_TRANSFORM);
                m_action = ACTION_ROTATION;

                return True;
            }
        }
    }

    return False;
}

o3d::Bool CameraManipulator::mouseReleaseEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (event.button(Mouse::LEFT)) {
        if (m_action != ACTION_NONE) {
            m_action = ACTION_NONE;
            masterScene->setActionMode(MasterScene::ACTION_NONE);

            return True;
        }
    }

    return False;
}

o3d::Bool CameraManipulator::mouseMoveEvent(const MouseEvent &event, MasterScene *masterScene)
{
    Float elapsed = masterScene->frameDuration();
    BaseNode *cameraNode = masterScene->camera()->getNode();

    if (m_action == ACTION_NONE || !cameraNode) {
        return False;
    }

    if (m_action == ACTION_ZOOM) {
        Float x = 0.f, y = 0.f, z = 0.f;

        x = masterScene->transformDelta().x() * 100.f * elapsed;
        z = masterScene->transformDelta().y() * 100.f * elapsed;

        // major axis from major delta
        if (o3d::abs(x) > o3d::abs(z)) {
            z = 0;
        } else {
            x = 0;
        }

        // slow motion if shift is down
        if (masterScene->motionType() == MasterScene::MOTION_PRECISE) {
            x *= 0.1;
            z *= 0.1;
        } else if (masterScene->motionType() == MasterScene::MOTION_FAST) {
            x *= 10;
            z *= 10;
        }

        cameraNode->getTransform()->translate(Vector3(x, y, z));

        return True;
    } else if (m_action == ACTION_ROTATION) {
        Float speed = 1;

        // slow rotation if shift is down
        if (masterScene->motionType() == MasterScene::MOTION_PRECISE) {
            speed = 0.1;
        } else if (masterScene->motionType() == MasterScene::MOTION_FAST) {
            speed = 10;
        }

        cameraNode->getTransform()->rotate(Y, -masterScene->transformDelta().x() * elapsed * speed);
        cameraNode->getTransform()->rotate(X, -masterScene->transformDelta().y() * elapsed * speed);

        return True;
    } else if (m_action == ACTION_TRANSLATION) {
        Float x = 0.f, y = 0.f, z = 0.f;

        x = -masterScene->transformDelta().x() * 100.f * elapsed;
        y = masterScene->transformDelta().y() * 100.f * elapsed;

        // slow motion if shift is down
        if (masterScene->motionType() == MasterScene::MOTION_PRECISE) {
            x *= 0.1;
            y *= 0.1;
        } else if (masterScene->motionType() == MasterScene::MOTION_FAST) {
            x *= 10;
            y *= 10;
        }

        cameraNode->getTransform()->translate(Vector3(x, y, z));

        return True;
    }

    return False;
}

o3d::Bool CameraManipulator::wheelEvent(const WheelEvent &event, MasterScene *masterScene)
{
    if (masterScene->actionMode() != MasterScene::ACTION_NONE) {
        return False;
    }

    Float elapsed = masterScene->frameDuration();
    BaseNode *cameraNode = masterScene->camera()->getNode();

    if (m_action != ACTION_NONE || !cameraNode) {
        return False;
    }

    Int32 deltaX = event.angleDelta().x();   // ALT + or second wheel axis
    Int32 deltaY = event.angleDelta().y();

    // camera zoom/rotate
    if (deltaY != 0 || deltaX != 0) {
        Float delta = (deltaX + deltaY) * 100.f / 120.f * elapsed;

        // change type or axis if modifier
        if (event.modifiers() & InputEvent::CTRL_MODIFIER && event.modifiers() & InputEvent::SHIFT_MODIFIER) {
            // rotate on Z
            cameraNode->getTransform()->rotate(Z, -delta * 0.1);
        } else if (event.modifiers() & InputEvent::ALT_MODIFIER && event.modifiers() & InputEvent::SHIFT_MODIFIER) {
            // rotate on X
            cameraNode->getTransform()->rotate(X, -delta * 0.1);
        } else if (event.modifiers() & InputEvent::ALT_MODIFIER && event.modifiers() & InputEvent::CTRL_MODIFIER) {
            // rotate on Y
            cameraNode->getTransform()->rotate(Y, -delta * 0.1);
        } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
            // translate on X
            cameraNode->getTransform()->translate(Vector3(delta * 10, 0, 0));
        } else if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
            // translate on Y
            cameraNode->getTransform()->translate(Vector3(0, delta * 10, 0));
        } else {
            // translate on Z
            cameraNode->getTransform()->translate(Vector3(0, 0, -delta * 10));
        }

        return True;
    }

    return False;
}

o3d::Bool CameraManipulator::mouseDoubleClickEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (event.button(Mouse::LEFT) && m_hoverPart == PART_CUBE) {
        // reset the transformation
        masterScene->camera()->getNode()->getTransform()->setRotation(Quaternion());

        return True;
    }

    return False;
}

/**
 * @brief Common UI scene hub manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-24
 * @details
 */

#include "o3d/studio/common/ui/scene/hubmanipulator.h"
#include "o3d/studio/common/workspace/structuralhub.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/drawinfo.h>
#include <o3d/engine/object/mtransform.h>
#include <o3d/engine/viewport.h>
#include <o3d/engine/hierarchy/node.h>

#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::common;


HubManipulator::HubManipulator(MasterScene *masterScene) :
    SceneUIElement(masterScene, SCENE_UI_3D, POST_DRAW, True),
    m_transform(new MTransform),
    m_displayScale(1),
    m_hoverAxe(AXE_NONE),
    m_activeAxe(AXE_NONE),
    m_transforming(False),
    m_transformMode(TRANSLATE),
    m_pivotPoint(PIVOT_INDIVIDUAL),
    m_transformOrientation(TR_LOCAL),
    m_actionRadius(0),
    m_activeElt(nullptr)
{

}

HubManipulator::~HubManipulator()
{
    delete m_transform;
}

void HubManipulator::setup(MasterScene *masterScene)
{
    // register the picking colors
    masterScene->registerPickingId(0xffffff00, this);
    masterScene->registerPickingId(0xffffff01, this);
    masterScene->registerPickingId(0xffffff02, this);
    masterScene->registerPickingId(0xffffff03, this);
}

void HubManipulator::release(MasterScene *masterScene)
{
    // register the picking colors
    masterScene->unregisterPickingId(0xffffff00);
    masterScene->unregisterPickingId(0xffffff01);
    masterScene->unregisterPickingId(0xffffff02);
    masterScene->unregisterPickingId(0xffffff03);
}

void HubManipulator::hover(o3d::UInt32 id, const o3d::Point3f &pos)
{
    O3D_UNUSED(pos);

    if (id == 0xffffff00) {
        m_hoverAxe = AXE_X;
    } else if (id == 0xffffff01) {
        m_hoverAxe = AXE_Y;
    } else if (id == 0xffffff02) {
        m_hoverAxe = AXE_Z;
    } else if (id == 0xffffff03) {
        m_hoverAxe = AXE_MANY;
    } else {
        m_hoverAxe = AXE_NONE;
    }
}

void HubManipulator::leave()
{
    m_hoverAxe = AXE_NONE;
}

void HubManipulator::setSelection(MasterScene *masterScene, const std::list<StructuralHub *> targets)
{
    m_targets = targets;

    // default take first hub as active element
    if (targets.size() > 0) {
        m_activeElt = m_targets.front();
    }

    updateTransform(masterScene, True);
}

void HubManipulator::setActiveHub(MasterScene *masterScene, StructuralHub *hub)
{
    m_activeElt = hub;
    updateTransform(masterScene, True);
}

o3d::Bool HubManipulator::hasSelection() const
{
    return m_targets.size() > 0;
}

StructuralHub *HubManipulator::activeHub()
{
    return m_activeElt;
}

void HubManipulator::refresh(MasterScene *masterScene)
{
    // outside of transform, refresh camera changes in view transform mode
    if (isTransform()) {
        return;
    }

    if (m_transformOrientation == TR_VIEW) {
        Quaternion rot;

        // aligned to camera
        rot = masterScene->camera()->getNode()->getTransform()->getRotation();

        // change initial rotation
        m_transform->setRotation(rot);

        // initial transformation for reset if cancel
        m_orgRot = m_transform->getRotation();
    }
}

o3d::Vector3f HubManipulator::computeLinearVelocity(
        MasterScene *masterScene,
        const o3d::Vector3f &delta,
        Axe axe)
{
    Vector3 v;

    const Box2i &vp = masterScene->viewPort();

    // compute 2d pos of the helper
    Vector3 helperPos1 = Matrix::projectPoint(
        masterScene->camera()->getProjectionMatrix(),
        masterScene->camera()->getModelviewMatrix() * m_transform->getMatrix(),
        vp,
        Vector3());

    Vector3 dir3d;

    if (axe == AXE_X) {
        dir3d.x() = 1;
    } else if (axe == AXE_Y) {
        dir3d.y() = 1;
    } else if (axe == AXE_Z) {
        dir3d.z() = 1;
    } else if (axe == AXE_MANY) {
        // @todo
    }

    // compute 2d pos of the helper plus a delta in the direction of the axes
    Vector3 helperPos2 = Matrix::projectPoint(
        masterScene->camera()->getProjectionMatrix(),
        masterScene->camera()->getModelviewMatrix() * m_transform->getMatrix(),
        vp,
        dir3d);

    Vector2f dir2d(helperPos2.x() - helperPos1.x(), helperPos2.y() - helperPos1.y());
    dir2d.normalize();

    Float velocity = dir2d * Vector2f(delta.x(), -delta.y());

    if (axe == AXE_X) {
        v.x() = velocity;
    } else if (axe == AXE_Y) {
        v.y() = velocity;
    } else if (axe == AXE_Z) {
        v.z() = velocity;
    } else if (axe == AXE_MANY) {
        // @todo
    }

    Application::instance()->messenger().status(
                String("Translation delta={0}").arg(m_relativeV + v));

    return v * 0.1f;
}

o3d::Vector3f HubManipulator::computeCircularVelocity(
        MasterScene *masterScene,
        const o3d::Vector3f &delta,
        Axe axe)
{
    Vector3 v;

    const Box2i &vp = masterScene->viewPort();

    // compute 2d pos of the center of the rotation axis
    Vector3 helperPos1 = Matrix::projectPoint(
                             masterScene->camera()->getProjectionMatrix(),
                             masterScene->camera()->getModelviewMatrix() * m_transform->getMatrix(),
                             vp,
                             Vector3());

    // and convert it in centered coordinates
    Vector2f center(
                helperPos1.x() - vp.width() / 2 + vp.x(),
                helperPos1.y() - vp.height() / 2 + vp.y());

    Vector2f target(
                (m_previous.x() + delta.x()) - vp.width() / 2 + vp.x(),
                (vp.height() - (m_previous.y() + delta.y())) - vp.height() / 2 + vp.y());

    Vector2f p1 = target - center;

    m_actionPos = Vector3f(target.x() + vp.width() / 2, target.y() + vp.height() / 2, 0);
    m_actionRadius = p1.length();

    Vector2f p2(
                m_previous.x() - vp.width() / 2 - vp.x(),
                (vp.height() - m_previous.y()) - vp.height() / 2 - vp.y());

    p2 -= center;

    p1.normalize();
    p2.normalize();

    // @todo according to the direction of the active object
    // minus the sign of the velocity if inverted to the natural 2d rotation

    // http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/
    Float phi = o3d::simplifyRadian(atan2(p1.y(), p1.x()) - atan2(p2.y(), p2.x()));
    Float velocity = phi;

    m_actionPhi = o3d::simplifyRadian(atan2(p1.y(), p1.x()) - atan2(1, 0));

    if (axe == AXE_X) {
        v.x() = velocity;
    } else if (axe == AXE_Y) {
        v.y() = velocity;
    } else if (axe == AXE_Z) {
        v.z() = velocity;
    } else if (axe == AXE_MANY) {
        // @todo
    }

    Vector3f rot(m_relativeV + v);
    for (UInt32 i = 0; i < 3; ++i) {
        rot[i] = o3d::toDegree(o3d::simplifyRadian(rot[i]));
    }

    Application::instance()->messenger().status(
                String("Manipulator radius={0}, Rotation delta={1}").arg(m_actionRadius).arg(rot));

    return v;
}

o3d::Bool HubManipulator::keyPressEvent(const KeyEvent &event, MasterScene *masterScene)
{
    if (isTransform()) {
        if (event.modifiers() == 0) {
            if (event.vKey() == o3d::VKey::KEY_ESCAPE) {
                // cancel current transformation
                cancelTransform(masterScene);
                masterScene->setActionMode(MasterScene::ACTION_NONE);

                return True;
            }
        }
    }

    return False;
}

o3d::Bool HubManipulator::keyReleaseEvent(const KeyEvent &event, MasterScene *masterScene)
{
    O3D_UNUSED(event);
    O3D_UNUSED(masterScene);

    if (isTransform()) {
        return True;
    }

    return False;
}

o3d::Bool HubManipulator::mousePressEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (!isTransform() && event.button(Mouse::LEFT)) {
        masterScene->setActionMode(MasterScene::ACTION_TRANSFORM);
        beginTransform(masterScene, Vector3f(event.localPos().x(), event.localPos().y(), 0));

        return True;
    }

    if (isTransform() && event.button(Mouse::RIGHT)) {
        cancelTransform(masterScene);
        masterScene->setActionMode(MasterScene::ACTION_NONE);

        return True;
    }

    return False;
}

o3d::Bool HubManipulator::mouseReleaseEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (isTransform() && event.button(Mouse::LEFT)) {
        endTransform(masterScene);
        masterScene->setActionMode(MasterScene::ACTION_NONE);

        return True;
    }

    return False;
}

o3d::Bool HubManipulator::mouseMoveEvent(const MouseEvent &event, MasterScene *masterScene)
{
    if (isTransform()) {
        Float x = 0.f, y = 0.f, z = 0.f;
        Float elapsed = masterScene->frameDuration();

        x = masterScene->transformDelta().x() * elapsed * 100;
        y = masterScene->transformDelta().y() * elapsed * 100;

        transform(Vector3(x, y, z), masterScene);

        return True;
    }

    return False;
}

void HubManipulator::beginTransform(MasterScene *masterScene, const Vector3f &pos)
{
    O3D_ASSERT(m_orgV.empty());
    O3D_ASSERT(m_orgQ.empty());

    // don't want the object local axis
    masterScene->scene()->setDrawObject(Scene::DRAW_LOCAL_AXIS, False);

    m_activeAxe = m_hoverAxe;
    m_focus = True;
    m_relativeV.zero();
    m_initial = m_previous = pos;
    m_transforming = True;

    updateTransform(masterScene, True);

    // m_pivotPoint = masterScene->transformPivotMode();
    // m_transformOrientation = masterScene->transformOrientationMode();

    if (masterScene->transformMode() == ROTATE) {
        m_transformMode = ROTATE;

        for (Hub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub);
                m_orgQ.push_back(spacialNode->transform(0)->getRotation());
            } else if (hub->isParentSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                m_orgQ.push_back(spacialNode->transform(0)->getRotation());
            } else {
                m_orgQ.push_back(Quaternion());
            }
        }

        // init some variables
        computeCircularVelocity(masterScene, Vector3f(), AXE_NONE);

    } else if (masterScene->transformMode() == TRANSLATE) {
        m_transformMode = TRANSLATE;

        for (Hub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub);
                m_orgV.push_back(spacialNode->transform(0)->getPosition());
            } else if (hub->isParentSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                m_orgV.push_back(spacialNode->transform(0)->getPosition());
            } else {
                m_orgV.push_back(Vector3());
            }
        }

        // init some variables
        computeLinearVelocity(masterScene, Vector3f(), AXE_NONE);

    } else if (masterScene->transformMode() == SCALE) {
        m_transformMode = SCALE;

        for (Hub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub);
                m_orgV.push_back(spacialNode->transform(0)->getScale());
            } else if (hub->isParentSpacialNode()) {
                SpacialNodeHub *spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                m_orgV.push_back(spacialNode->transform(0)->getScale());
            } else {
                m_orgV.push_back(Vector3(1, 1, 1));
            }
        }

        // init some variables
        computeLinearVelocity(masterScene, Vector3f(), AXE_NONE);

    }  else if (masterScene->transformMode() == SKEW) {
       m_transformMode = SKEW;

       // init some variables
       computeLinearVelocity(masterScene, Vector3f(), AXE_NONE);
    }
}

void HubManipulator::transform(const o3d::Vector3f &v, MasterScene *masterScene)
{
    Float s = 1.f;

    if (masterScene->motionType() == MasterScene::MOTION_FAST) {
        s *= 10.f;
    } else if (masterScene->motionType() == MasterScene::MOTION_PRECISE) {
        s *= 0.1f;
    } else if (masterScene->motionType() == MasterScene::MOTION_STEP) {
        // s *= 0.1f;  // @todo step by N
    } else if (masterScene->motionType() == MasterScene::MOTION_GRID) {
        // s *= 0.1f;  // @todo align to virtual grid
    } else if (masterScene->motionType() == MasterScene::MOTION_MAGNET) {
        // s *= 0.1f;  // @todo magnet to nearest location
    }

    if (!m_activeElt) {
        return;
    }

    // @todo determin the amount of x and y to be colinear or at 90° to the axis of the transform
    if (m_transformMode == ROTATE) {
        // compute the relative change from origins in the direction of the arc of the axe and the input delta
        m_relativeV += computeCircularVelocity(masterScene, v * s, m_activeAxe);

        for (UInt32 i = 0; i < 3; ++i) {
            m_relativeV[i] = o3d::simplifyRadian(m_relativeV[i]);
        }

        Quaternion q;
        q.fromEuler(m_relativeV);
        q.normalize();
        m_transform->setRotation(m_orgRot * q);

        auto it = m_orgQ.begin();
        SpacialNodeHub *spacialNode;
        Quaternion pivotAxe;
        Vector3 pivotPoint;

        // @todo
        if (m_pivotPoint == PIVOT_ACTIVE_ELT) {
            pivotAxe = m_activeElt->transform(0)->getRotation();
            pivotPoint = m_activeElt->transform(0)->getPosition();
            // @todo translate
        } else if (m_pivotPoint == PIVOT_INDIVIDUAL) {
            if (m_transformOrientation == TR_GLOBAL) {
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        q = m_transform->getRotation();
                        spacialNode->setRotation(0, (*it) * q);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }
            } else if (m_transformOrientation == TR_LOCAL) {
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        spacialNode->setRotation(0, (*it) * q);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }
            } else if (m_transformOrientation == TR_VIEW) {
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        q = m_transform->getRotation();
                        spacialNode->setRotation(0, (*it) * q);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }
            }
        } else if (m_pivotPoint == PIVOT_MEDIAN) {
            pivotAxe = m_activeElt->transform(0)->getRotation();
            Vector3 pivotPoint = m_transform->getPosition();
            // @todo compute for each element the distance to the pivot point
        } else if (m_pivotPoint == PIVOT_USER) {
            // @todo
            // pivot = masterScene->helperPoint().transform().getRotation();
        }
    } else if (m_transformMode == TRANSLATE) {
        // compute the relative change from origins in the direction of the axe and the input delta
        m_relativeV += computeLinearVelocity(masterScene, v * s, m_activeAxe);

        auto it = m_orgV.begin();
        SpacialNodeHub *spacialNode;
        Quaternion pivotAxe;
        Vector3 pivotPoint;
        Vector3 pos;

        if (m_pivotPoint == PIVOT_ACTIVE_ELT) {
            pivotAxe = m_activeElt->transform(0)->getRotation();
            pivotPoint = m_activeElt->transform(0)->getPosition();

            // pivot active + transform global
            // @todo

            // pivot active + transform local
            // @todo

            // pivot active + transform view
            // @todo

        } else if (m_pivotPoint == PIVOT_INDIVIDUAL) {
            if (m_transformOrientation == TR_GLOBAL) {
                // pivot individual + transform global
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        // axis is aligned to origin, relative translation
                        pos = m_relativeV;

                        spacialNode->setPosition(0, (*it) + pos);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // axis is aligned to origin, relative translation
                pos = m_relativeV;
                m_transform->setPosition(m_orgPos + pos);
                m_transform->setRotation(Quaternion());  // useless

            } else if (m_transformOrientation == TR_LOCAL) {
                // pivot individual + transform local
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        // axis is naturally aligned to object, relative translation
                        pos = m_relativeV;

                        spacialNode->transform(0)->getRotation().transform(pos);
                        spacialNode->setPosition(0, (*it) + pos);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // helper transform, aligned to active element
                pos = m_relativeV;
                m_transform->getRotation().transform(pos);
                m_transform->setPosition(m_orgPos + pos);
                m_transform->setRotation(m_activeElt->transform(0)->getRotation());

            } else if (m_transformOrientation == TR_VIEW) {
                // pivot individual + transform view
                pivotAxe = masterScene->camera()->getNode()->getTransform()->getRotation();

                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        // axis is aligned to view, relative translation
                        pos = m_relativeV;
                        pivotAxe.transform(pos);

                        spacialNode->setPosition(0, (*it) + pos);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // axis is aligned to view, relative translation
                pos = m_relativeV;
                pivotAxe.transform(pos);
                m_transform->setPosition(m_orgPos + pos);
            }
        } else if (m_pivotPoint == PIVOT_MEDIAN) {
            pivotAxe = m_activeElt->transform(0)->getRotation();
            Vector3 pivotPoint = m_transform->getPosition();
            // @todo compute for each element the distance to the pivot point (only for rotation)

            // pivot median + transform global
            // @todo

            // pivot median + transform local
            // @todo

            // pivot median + transform view
            // @todo

        } else if (m_pivotPoint == PIVOT_USER) {
            // @todo
            // pivot = masterScene->helperPoint().transform().getRotation();

            // pivot user + transform global
            // @todo

            // pivot user + transform local
            // @todo

            // pivot user + transform view
            // @todo
        }
    } else if (m_transformMode == SCALE) {
        // compute the relative change from origins in the direction of the axe and the input delta
        Vector3 relativeV = computeLinearVelocity(masterScene, v * s, m_activeAxe);

        // no zero, need 1 for non concerned axes
        if (m_activeAxe == AXE_X) {
            m_relativeV.x() += relativeV.x();
            m_relativeV.y() = 1;
            m_relativeV.z() = 1;
        } else if (m_activeAxe == AXE_Y) {
            m_relativeV.x() = 1;
            m_relativeV.y() += relativeV.y();
            m_relativeV.z() = 1;
        } else if (m_activeAxe == AXE_Z) {
            m_relativeV.x() = 1;
            m_relativeV.y() = 1;
            m_relativeV.z() += relativeV.z();
        } else if (m_activeAxe == AXE_MANY) {
            // @todo
        }

        Vector3 scale;

        auto it = m_orgV.begin();
        SpacialNodeHub *spacialNode;       

        if (m_pivotPoint == PIVOT_ACTIVE_ELT) {
            // pivotAxe = m_activeElt->transform(0)->getRotation();
            // pivotPoint = m_activeElt->transform(0)->getPosition();

            // pivot active + transform global
            // @todo

            // pivot active + transform local
            // @todo

            // pivot active + transform view
            // @todo

        } else if (m_pivotPoint == PIVOT_INDIVIDUAL) {
            if (m_transformOrientation == TR_GLOBAL) {
                // pivot individual + transform global
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        scale = m_relativeV;
                        spacialNode->setScale(0, (*it) + scale);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // to be consistent during translation
                m_transform->setScale(m_orgScale + scale);

            } else if (m_transformOrientation == TR_LOCAL) {
                // pivot individual + transform local
                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        scale = m_relativeV;
                        spacialNode->setScale(0, (*it) + scale);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // to be consistent during translation
                m_transform->setScale(m_orgScale + scale);

            } else if (m_transformOrientation == TR_VIEW) {
                // pivot individual + transform view
                // pivotAxe = masterScene->cameraTransform().getRotation();

                for (StructuralHub* hub : m_targets) {
                    if (hub->isSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub);
                    } else if (hub->isParentSpacialNode()) {
                        spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                    } else {
                        spacialNode = nullptr;
                    }

                    if (spacialNode) {
                        scale = m_relativeV;
                        spacialNode->setScale(0, (*it) + scale);

                        SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                        masterScene->addCommand(sceneCommand);
                    }

                    ++it;
                }

                // to be consistent during translation
                m_transform->setScale(m_orgScale + scale);
            }
        } else if (m_pivotPoint == PIVOT_MEDIAN) {
            // pivotAxe = m_activeElt->transform(0)->getRotation();
            // Vector3 pivotPoint = m_transform->getPosition();
            // @todo compute for each element the distance to the pivot point (only for rotation)

            // pivot median + transform global
            // @todo

            // pivot median + transform local
            // @todo

            // pivot median + transform view
            // @todo

        } else if (m_pivotPoint == PIVOT_USER) {
            // @todo
            // pivot = masterScene->helperPoint().transform().getRotation();

            // pivot user + transform global
            // @todo

            // pivot user + transform local
            // @todo

            // pivot user + transform view
            // @todo
        }
    }  else if (m_transformMode == SKEW) {
        // @todo
    }

    // update global input pos
    m_previous += v;
}

void HubManipulator::endTransform(MasterScene *masterScene)
{
    // restore drawing of local axis
    masterScene->scene()->setDrawObject(Scene::DRAW_LOCAL_AXIS, True);

    m_transformMode = (TransformMode)masterScene->transformMode();
    m_activeAxe = AXE_NONE;
    m_focus = False;
    m_transforming = False;
    m_relativeV.zero();
    m_previous.zero();
    m_orgV.clear();
    m_orgQ.clear();
}

void HubManipulator::cancelTransform(MasterScene *masterScene)
{
    SpacialNodeHub *spacialNode;

    // reset transforms of each target
    if (m_transformMode == TRANSLATE) {
        auto it = m_orgV.begin();
        for (StructuralHub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
            } else {
                spacialNode = nullptr;
            }

            if (spacialNode) {
                SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                spacialNode->setPosition(0, *it);
                masterScene->addCommand(sceneCommand);
            }

            ++it;
        }
    } else if (m_transformMode == ROTATE) {
        auto it = m_orgQ.begin();
        for (StructuralHub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
            } else {
                spacialNode = nullptr;
            }

            if (spacialNode) {
                SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                spacialNode->setRotation(0, *it);
                masterScene->addCommand(sceneCommand);
            }

            ++it;
        }
    } else if (m_transformMode == SCALE) {
        auto it = m_orgV.begin();
        for (StructuralHub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
            } else {
                spacialNode = nullptr;
            }

            if (spacialNode) {
                SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
                spacialNode->setScale(0, *it);
                masterScene->addCommand(sceneCommand);
            }

            ++it;
        }
    } else if (m_transformMode == SKEW) {
//        auto it = m_orgV.begin();
//        for (StructuralHub* hub : m_targets) {
//            if (hub->isSpacialNode()) {
//                spacialNode = static_cast<SpacialNodeHub*>(hub);
//            } else if (hub->isParentSpacialNode()) {
//                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
//            } else {
//                spacialNode = nullptr;
//            }
//
//            if (spacialNode) {
//                SceneHubCommand *sceneCommand = new SceneHubCommand(spacialNode, SceneHubCommand::SYNC);
//                spacialNode->setSkew(0, *it);
//                masterScene->addCommand(sceneCommand);
//            }
//
//            ++it;
//        }
    }

    m_transform->setPosition(m_orgPos);
    m_transform->setRotation(m_orgRot);
    m_transform->setScale(m_orgScale);

    m_transformMode = (TransformMode)masterScene->transformMode();
    m_activeAxe = AXE_NONE;
    m_focus = False;
    m_transforming = False;
    m_relativeV.zero();
    m_orgV.clear();
    m_orgQ.clear();
}

o3d::Bool HubManipulator::isTransform() const
{
    return m_transforming;
}

o3d::Vector3f HubManipulator::currentTransform() const
{
    if (!isTransform() && m_transformMode == TRANSLATE) {
        return m_relativeV; // + m_orgPos;
    } else {
        return m_relativeV;
    }
}

void HubManipulator::setPosition(const o3d::Vector3f &pos)
{
    if (!isTransform() && m_transformMode == TRANSLATE) {
        Vector3f p = pos; // - m_orgPos;
        beginTransform(masterScene(), Vector3f());
        m_relativeV = p;
        //updateTransform(masterScene(), False);
        transform(Vector3f(), masterScene());
        endTransform(masterScene());
    }
}

void HubManipulator::setRotation(const o3d::Vector3f &euler)
{
    if (!isTransform() && m_transformMode == ROTATE) {
        m_relativeV = euler;
        transform(Vector3f(), masterScene());
    }
}

void HubManipulator::setScale(const o3d::Vector3f &scale)
{
    if (!isTransform() && m_transformMode == SCALE) {
        m_relativeV = scale;
        transform(Vector3f(), masterScene());
    }
}

void HubManipulator::setSkew(const o3d::Vector3f &skew)
{
    if (!isTransform() && m_transformMode == SKEW) {
        m_relativeV = skew;
        transform(Vector3f(), masterScene());
    }
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

    if (m_targets.empty()) {
        // no targets no display
        return;
    }

    // @todo remove need a listener to know about a changes on targets
    // updateTransform(masterScene, False);

    // during display uses current active viewport and camera
    refresh(masterScene);

    PrimitiveAccess primitive = scene->getPrimitiveManager()->access(drawInfo);
    Context &context = *scene->getContext();

    // used only in ambiant mode because invalid in a picking usage
    const Box2i &vp = context.getViewPort();

    // biaised depth
    context.enableDepthTest();
    context.enableDepthWrite();
    context.setDepthRange(0.0, 0.1);

    // computed visibility and 2d mapped position (using window viewport)
    Vector3 v = Matrix::projectPoint(
                    scene->getActiveCamera()->getProjectionMatrix(),
                    scene->getActiveCamera()->getModelviewMatrix() * m_transform->getMatrix(),
                    masterScene->viewPort(),
                    Vector3());

    // outside
    if (v.z() < 0) {
        return;
    }

    Vector3 dist = m_transform->getMatrix().getTranslation() - scene->getActiveCamera()->getAbsoluteMatrix().getTranslation();
    Float s = m_displayScale * dist.length() * 0.1;

    // in ortho mode we need to adjust using projection matrix
    if (scene->getActiveCamera()->isOrtho()) {
        Float w = (scene->getActiveCamera()->getRight() - scene->getActiveCamera()->getLeft());
        s = m_displayScale * w * 0.075;
    }

    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix() * m_transform->getMatrix());
    primitive->modelView().push();

    if (drawInfo.pass == DrawInfo::PICKING_PASS) {
        // assume picking project matrix is always current

        // a picking pass for manipulation
        if (m_transformMode == ROTATE) {
            // @todo the axis the most colinear to the plane avix must be the external circle

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
        } else if (m_transformMode == TRANSLATE) {
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
        } else if (m_transformMode == SCALE) {
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
        } else if (m_transformMode == SKEW) {

        }
    } else if (drawInfo.pass == DrawInfo::AMBIENT_PASS) {
        Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);  // AA_HINT_NICEST

        // active camera project matrix
        scene->getActiveCamera()->setProjectionMatrix();

        primitive->setColor(1, 1, 1);

        // @todo an highlighted bounding volume...

        // an highlighted version of the targets @todo but might need a particular status on targeted hubs

        if (m_transformMode == ROTATE) {
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
        } else if (m_transformMode == TRANSLATE) {
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
        } else if (m_transformMode == SCALE) {
            // scale axes with squares
            // @todo 0.5a square in direction and proportional to current scale

            // translation axes
            primitive->setModelviewProjection();
            primitive->drawXYZAxis(Vector3(s, s, s));

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(s*0.95f, 0, 0));
            primitive->modelView().rotateZ(o3d::toRadian(-90.f));
            primitive->setColor(axeColor(AXE_X));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.15f,s*0.15f,s*0.15f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,s*0.95f,0));
            primitive->setColor(axeColor(AXE_Y));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.15f,s*0.15f,s*0.15f));
            primitive->modelView().pop();

            primitive->modelView().push();
            primitive->modelView().translate(Vector3(0,0,s*0.95f));
            primitive->modelView().rotateX(o3d::toRadian(90.f));
            primitive->setColor(axeColor(AXE_Z));
            primitive->draw(PrimitiveManager::SOLID_CUBE1, Vector3(s*0.15f,s*0.15f,s*0.15f));
            primitive->modelView().pop();
        } else if (m_transformMode == SKEW) {
            // @todo
        }

        // display helper radius
        if (m_actionRadius != 0.0 && m_transforming) {
            if (m_transformMode == ROTATE || m_transformMode == SCALE || m_transformMode == SKEW) {
                m_actionPos.set(m_previous.x(),
                                vp.height() - m_previous.y(),
                                0);

                // ortho projection
                Matrix4 pj;
                pj.buildOrtho(vp.x(), vp.x2(), vp.y(), vp.y2(), -1, 1);
                primitive->projection().set(pj);

                // identiy modelview
                primitive->modelView().identity();

                Vector3f p1(v.x(), v.y(), 0);
                Vector3f p2(m_actionPos.x(), m_actionPos.y(), 0);
                Vector3f p3(m_actionPos.x() - 5, m_actionPos.y() - 5, 0);
                Vector3f p4(m_actionPos.x() + 5, m_actionPos.y() - 5, 0);

                // angle from origin, draw an arrow
                Matrix3 rot;
                rot.rotateZ(m_actionPhi);

                p3 = rot * Vector3f(-5, -5, 0) + p2;
                p4 = rot * Vector3f(5, -5, 0) + p2;

//                Float sin = sinf(-m_actionPhi);
//                Float cos = cosf(-m_actionPhi);

//                p3.x() = -5 * cos + -5 * sin + p2.x();
//                p3.y() = -5 * cos - -5 * sin + p2.y();

//                p4.x() = 5 * cos + -5 * sin + p2.x();
//                p4.y() = -5 * cos - 5 * sin + p2.y();

                primitive->setColor(Color(0, 0, 0, 1));

                primitive->modelView().push();
                primitive->beginDraw(P_LINES);
                primitive->addVertex(p1, Color(0, 0, 0, 1));
                primitive->addVertex(p2, Color(0, 0, 0, 1));

                primitive->addVertex(p2, Color(0, 0, 0, 1));
                primitive->addVertex(p3, Color(0, 0, 0, 1));

                primitive->addVertex(p2, Color(0, 0, 0, 1));
                primitive->addVertex(p4, Color(0, 0, 0, 1));
                primitive->endDraw();
                primitive->modelView().pop();
            }
        }

        context.setAntiAliasing(aa);
    }

    primitive->modelView().pop();

    // restore
    context.setDefaultDepthWrite();
    context.setDefaultDepthTest();
    context.setDefaultDepthRange();
}

void HubManipulator::setPivotPoint(HubManipulator::PivotPoint mode)
{
    if (!isTransform()) {
        m_pivotPoint = mode;
        updateTransform(masterScene(), False);
    }
}

void HubManipulator::setTransformOrientation(HubManipulator::TransformOrientation mode)
{
    if (!isTransform()) {
        m_transformOrientation = mode;
        updateTransform(masterScene(), False);
    }
}

void HubManipulator::setTransforMode(HubManipulator::TransformMode mode)
{
    if (!isTransform()) {
        m_transformMode = mode;
        updateTransform(masterScene(), False);
    }
}

void HubManipulator::updateTransform(MasterScene *masterScene, Bool keepOrg)
{
    if (m_targets.empty()) {
        m_transform->identity();
        return;
    }

    Vector3 pos;
    Quaternion rot;

    // active element of the selection (at now the last)
    StructuralHub *hub = m_targets.back();
    if (hub->isSpacialNode()) {
        m_activeElt = static_cast<SpacialNodeHub*>(hub);
    } else if (hub->isParentSpacialNode()) {
        m_activeElt = static_cast<SpacialNodeHub*>(hub->parent());
    }

    if (!m_activeElt) {
        return;
    }

    if (m_transformOrientation == TR_GLOBAL) {
        // aligned to origin
        rot.identity();
    } else if (m_transformOrientation == TR_LOCAL) {
        // axis from active element
        rot = m_activeElt->transform(0)->getRotation();
    } else if (m_transformOrientation == TR_VIEW) {
        // aligned to camera
        rot = masterScene->camera()->getNode()->getTransform()->getRotation();
    }

    if (m_pivotPoint == PIVOT_ACTIVE_ELT) {
        // position from active element
        pos = m_activeElt->transform(0)->getPosition();
    } else if (m_pivotPoint == PIVOT_INDIVIDUAL) {
        // position computed individualy by helper offset by median
        m_offset.zero();
        UInt32 c = 0;

        // compute the median position
        SpacialNodeHub *spacialNode;
        for (StructuralHub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
                m_offset += spacialNode->transform(0)->getPosition();
                ++c;
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                m_offset += spacialNode->transform(0)->getPosition();
                ++c;
            }
        }

        if (c) {
            m_offset *= 1.f / c;
        }

        // position from origin of individual elements
        pos.zero();

        // @todo improve sep of helper and transform and objects

        // position from active element
        pos = m_activeElt->transform(0)->getPosition();

    } else if (m_pivotPoint == PIVOT_MEDIAN) {
        // position computed to their median
        UInt32 c = 0;

        // compute the median position
        SpacialNodeHub *spacialNode;
        for (StructuralHub* hub : m_targets) {
            if (hub->isSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub);
                pos += spacialNode->transform(0)->getPosition();
                ++c;
            } else if (hub->isParentSpacialNode()) {
                spacialNode = static_cast<SpacialNodeHub*>(hub->parent());
                pos += spacialNode->transform(0)->getPosition();
                ++c;
            }
        }

        if (c) {
            pos *= 1.f / c;
        }
    } else if (m_pivotPoint == PIVOT_USER) {
        // @todo user defined from HelperPointManipulator
    }

    // setup initial transformation
    m_transform->identity();
    m_transform->setRotation(rot);
    m_transform->setPosition(pos);

    if (keepOrg) {
        // initial transformation for reset if cancel
        m_orgPos = m_transform->getPosition();
        m_orgRot = m_transform->getRotation();
        m_orgScale = m_transform->getScale();
    }
}

o3d::Color HubManipulator::axeColor(HubManipulator::Axe axe)
{
    Axe usedAxe = m_hoverAxe;
    if (isTransform()) {
        usedAxe = m_activeAxe;
    }

    if (usedAxe == AXE_MANY) {
        // @todo depends...
    } else if (usedAxe == axe) {
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
            return Color(0.f, 0.6f, 0.f);
        } else if (axe == AXE_Z) {
           return Color(0.f, 0.f, 1.f);
        }
    }

    return Color();
}

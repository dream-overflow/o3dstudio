/**
 * @brief Workspace master scene rendering and management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/ui/qtrenderer.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/workspace/scenecommand.h"
#include "o3d/studio/common/workspace/masterscenedrawer.h"
#include "o3d/studio/common/workspace/roothub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/structuralhub.h"

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"
#include "o3d/studio/common/ui/scene/hubmanipulator.h"
#include "o3d/studio/common/ui/toolbar/transformtoolbar.h"

#include <o3d/engine/glextdefines.h>
#include <o3d/engine/glextensionmanager.h>

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/viewportmanager.h>
#include <o3d/engine/screenviewport.h>
#include <o3d/engine/shadow/shadowvolumeforward.h>
#include <o3d/engine/hierarchy/hierarchytree.h>
#include <o3d/engine/utils/framemanager.h>
#include <o3d/engine/object/stransform.h>
#include <o3d/engine/context.h>
#include <o3d/engine/picking.h>
#include <o3d/engine/visibility/visibilitymanager.h>
#include <o3d/engine/visibility/visibilitybasic.h>

#include <o3d/core/keyboard.h>

#include <o3d/gui/gui.h>
#include <o3d/gui/fontmanager.h>
#include <o3d/gui/truetypefont.h>

#include "o3d/studio/common/ui/scene/grid.h"
#include "o3d/studio/common/ui/scene/infohud.h"
#include "o3d/studio/common/ui/scene/cameramanipulator.h"

#include "o3d/studio/common/messenger.h"

#include "o3d/studio/common/ui/keyevent.h"
//#include "o3d/studio/common/ui/mouseevent.h"
//#include "o3d/studio/common/ui/focusevent.h"

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLContext>

using namespace o3d::studio::common;


MasterScene::MasterScene(Entity *parent) :
    BaseObject(nullptr),
    m_parent(parent),
    m_content(nullptr),
    m_renderer(nullptr),
    m_scene(nullptr),
    m_actionMode(ACTION_NONE),
    m_prevActionMode(ACTION_NONE),
    m_motionType(MOTION_FOLLOW),
    m_transformMode(0),
    m_pivotMode(0),
    m_orientationMode(0),
    m_hoverHub(nullptr),
    m_camera(this),
    m_viewport(this),
    m_sceneDrawer(this),
    m_cameraManipulator(nullptr),
    m_hubManipulator(nullptr),
    m_hoverUIElement(nullptr)
{
    O3D_ASSERT(m_parent != nullptr);

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &MasterScene::onSelectionChanged);
}

MasterScene::~MasterScene()
{
    terminateDrawer();

    if (m_content && m_renderer) {
        m_content->makeCurrent();
        delete m_renderer;
        m_content->doneCurrent();
    } else if (m_renderer) {
        deletePtr(m_renderer);
    }

    // and finally widget
    if (m_content) {
        m_content->setDrawer(nullptr);

        common::UiController &uiCtrl = common::Application::instance()->ui();
        uiCtrl.removeContent(m_content);

        m_content->deleteLater();
    }
}

const Entity *MasterScene::parent() const
{
    return m_parent;
}

Entity *MasterScene::parent()
{
    return m_parent;
}

const Project *MasterScene::project() const
{
    return m_parent->project();
}

Project *MasterScene::project()
{
    return m_parent->project();
}

o3d::Renderer *MasterScene::renderer()
{
    return m_renderer;
}

O3DCanvasContent* MasterScene::widget()
{
    return m_content;
}

o3d::Scene *MasterScene::scene()
{
    return m_scene;
}

const o3d::Scene *MasterScene::scene() const
{
    return m_scene;
}

O3DCanvasContent *MasterScene::content()
{
    return m_content;
}

const O3DCanvasContent *MasterScene::content() const
{
    return m_content;
}

Hub *MasterScene::hoverHub()
{
    return m_hoverHub;
}

const Hub *MasterScene::hoverHub() const
{
    return m_hoverHub;
}

void MasterScene::addCommand(SceneCommand *command)
{
    if (command) {
        // m_commands.push_front(command);
        m_commands.push_back(command);

        // need refresh to process
        if (m_content) {
            m_content->queryRefresh();
        }
    }
}

void MasterScene::addSceneUIElement(SceneUIElement *elt)
{
    if (!m_sceneDrawer.isValid()) {
        O3D_ERROR(E_InvalidPrecondition("The master scene must be initialized before adding scene UI elements"));
    }

    auto it = std::find(m_sceneUIElements.begin(), m_sceneUIElements.end(), elt);
    if (it != m_sceneUIElements.end()) {
        O3D_ERROR(E_ValueRedefinition("Redefinition of a SceneUIElement"));
    }

    elt->setup(this);

    m_sceneUIElements.push_back(elt);

    // add to the scene drawer to be displayed
    m_sceneDrawer.get()->addSceneUIElement(elt);
}

void MasterScene::removeSceneUIElement(SceneUIElement *elt)
{
    auto it = std::find(m_sceneUIElements.begin(), m_sceneUIElements.end(), elt);
    if (it != m_sceneUIElements.end()) {
        elt->release(this);

        // remove from the scene drawer if initialized
        if (m_sceneDrawer.isValid()) {
            m_sceneDrawer.get()->removeSceneUIElement(*it);
        }

        // null if hover element
        if (m_hoverUIElement == elt) {
            m_hoverUIElement = nullptr;
        }

        m_sceneUIElements.erase(it);
    }
}

SceneUIElement *MasterScene::hubManipulator()
{
    return m_hubManipulator;
}

SceneUIElement *MasterScene::hoverSceneUIElement()
{
    return m_hoverUIElement;
}

const o3d::Camera* MasterScene::camera() const
{
    return m_camera.get();
}

o3d::Box2i MasterScene::viewPort() const
{
    return o3d::Box2i(0, 0, m_content->size().width(), m_content->size().height());
}

void MasterScene::initialize(Bool debug)
{
    if (m_content || m_scene || m_renderer) {
        return;
    }

    // master OpenGL canvas content
    m_content = new common::O3DCanvasContent(project()->ref().light(), debug);
    m_renderer = new common::QtRenderer(m_content);
    m_content->setDrawer(this);
    m_content->setRenderer(m_renderer);

    // no manipulation at initialization
    O3D_ASSERT(m_hubManipulator == nullptr);
    O3D_ASSERT(m_cameraManipulator == nullptr);

    common::UiController &uiCtrl = common::Application::instance()->ui();
    uiCtrl.addContent(m_content);
}

void MasterScene::paintDrawer()
{
    if (m_scene) {
        // CPU and GPU, need GL context
        m_scene->display();

        // process commands (FIFO)
        processCommands();

    }
}

void MasterScene::updateDrawer()
{
    if (m_scene) {
        // pure CPU, no GL context
        m_scene->update();
    }
}

void MasterScene::resizeDrawer(int w, int h)
{
    if (m_scene) {
        m_scene->reshape(w, h);
    }
}

void MasterScene::terminateDrawer()
{
    // make current context
    if (m_content) {
        m_content->makeCurrent();

        // process remaining commands
        processCommands();

        // remove any scene ui elements from the scene
        for (SceneUIElement *elt : m_sceneUIElements) {
            if (!elt->isDirectDraw()) {
                elt->removeFromScene(this);
            }
        }
    }

    // delete remaining commands (should be normally deleted just above)
    // but if the context is missing we have to do that
    if (!m_commands.empty()) {
        for (SceneCommand *cmd : m_commands) {
            delete cmd;
        }

        m_commands.clear();
    }

    // remove the scene ui elements
    if (!m_sceneUIElements.empty()) {
        for (SceneUIElement *elt : m_sceneUIElements) {
            // remove it from the scene drawer
            // m_sceneDrawer.get()->removeSceneUIElement(elt);

            delete elt;
        }

        // and from the master scene
        m_sceneUIElements.clear();
    }

    // deleted by the previous iteration
    m_cameraManipulator = nullptr;
    m_hubManipulator = nullptr;
    m_hoverUIElement = nullptr;

    // clear the scene
    if (m_scene) {
        // m_scene->getHierarchyTree()->getRootNode()->removeSon(*m_camera);
        // deletePtr(m_camera->getNode());

        m_camera = nullptr;
        m_viewport = nullptr;
        m_sceneDrawer = nullptr;

        delete m_scene;
        m_scene = nullptr;
    }

    // done with context
    if (m_content) {
        m_content->doneCurrent();
    }
}

o3d::Bool MasterScene::mousePressEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // keep pointer position
    m_pointerPos.set(event.localPos().x(), event.localPos().y(), 0);

    if (event.button(Mouse::LEFT)) {
        if (m_actionMode == ACTION_NONE) {
            if (m_hoverUIElement && m_hubManipulator == m_hoverUIElement) {
                // action using the manipulator
                setActionMode(ACTION_TRANSFORM);
                m_hubManipulator->beginTransform(this, Vector3f(event.localPos().x(), event.localPos().y(), 0));
            } else {
                // camera or world transform
                if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
                    setActionMode(ACTION_CAMERA_TRANSLATION);

                    // default normal speed, press again shift to slow
                    m_motionType = MOTION_FOLLOW;
                } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
                    setActionMode(ACTION_CAMERA_ZOOM);
                } else {
                    setActionMode(ACTION_CAMERA_ROTATION);
                }
            }
        }
    } else if (event.button(Mouse::MIDDLE)) {
        // nothing for now
    } else if (event.button(Mouse::RIGHT)) {
        if (m_actionMode == ACTION_TRANSFORM) {
            // cancel current transformation
            if (m_hubManipulator && m_hubManipulator->isTransform()) {
                m_hubManipulator->cancelTransform(this);
            }

            setActionMode(ACTION_NONE);
        } else if (m_hoverHub) {
            // selection on right button
            Selection &selection = Application::instance()->selection();

            Entity *entity = nullptr, *hover = nullptr;
            hover = findSelectable(m_hoverHub);

            // @todo for now auto defines accepted role to structural from view
            selection.setAcceptedRole(common::Selection::ACCEPT_STRUCTURAL_HUB);

            // in a multiple selection
            if (event.modifiers() & InputEvent::SHIFT_MODIFIER && hover) {
                // get current selection and remove from it
                auto previous = selection.filterCurrentByBaseType(TypeRef::hub());

                Application::instance()->selection().beginSelection();

                for (SelectionItem *item : previous) {
                    entity = item->entity();

                    if ((item->ref() != hover->ref().light()) && (entity = findSelectable(entity))) {
                        selection.appendSelection(entity);
                    }
                }

                if (!hover->isSelected() && (entity = findSelectable(m_hoverHub))) {
                    // if not selected add it to last
                    selection.appendSelection(entity);
                }

                Application::instance()->selection().endSelection();
            } else if (hover) {
                // single selection
                if (hover->isSelected()) {
                    // clear selection
                    selection.unselectAll();
                } else {
                    // initial selection
                    selection.select(hover);
                }
            }
        }
    }

    Bool hideCursor = False;

    if (m_actionMode == ACTION_CAMERA_ROTATION ||
            m_actionMode == ACTION_CAMERA_TRANSLATION ||
            m_actionMode == ACTION_CAMERA_ZOOM) {
        hideCursor = True;
    } else if (m_actionMode == ACTION_TRANSFORM) {
        hideCursor = True;
    }

    if (hideCursor) {
        // no cursor (and infinite scrolling)
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::BlankCursor);

        // initial relative
        m_content->setCursor(cursor);
        m_lockedPos = event.globalPos();
    }

    // could be necessary
    postPicking(m_pointerPos);

    return True;
}

o3d::Bool MasterScene::mouseReleaseEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // keep pointer position
    m_pointerPos.set(event.localPos().x(), event.localPos().y(), 0);

    if (event.button(Mouse::LEFT)) {
        if (m_prevActionMode == ACTION_TRANSFORM) {
            // nothing to do, prevent undefined behaviors
        } else if (m_actionMode == ACTION_NONE) {
            // nothing to do for now
        } else if (m_actionMode == ACTION_TRANSFORM) {
            if (m_hubManipulator && m_hubManipulator->isTransform()) {
                // action using the manipulator
                HubManipulator *hubManipulator = static_cast<HubManipulator*>(m_hubManipulator);
                hubManipulator->endTransform(this);
            }
            setActionMode(ACTION_NONE);
        } else if (m_actionMode == ACTION_CAMERA_ROTATION ||
                   m_actionMode == ACTION_CAMERA_TRANSLATION ||
                   m_actionMode == ACTION_CAMERA_ZOOM) {
            // stop camera action
            setActionMode(ACTION_NONE);
        }
    } else if (event.button(Mouse::MIDDLE)) {
        // nothing for now
    } else if (event.button(Mouse::RIGHT)) {
        // nothing for now
    }

    Bool showCursor = False;

    if (m_actionMode != ACTION_CAMERA_ROTATION &&
        m_actionMode != ACTION_CAMERA_TRANSLATION &&
        m_actionMode != ACTION_CAMERA_ZOOM &&
        m_actionMode != ACTION_TRANSFORM) {

        showCursor = True;
    }

    if (showCursor) {
        // restore cursor
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::ArrowCursor);

        m_content->setCursor(cursor);
    }

    // could be necessary
    postPicking(m_pointerPos);

    return True;
}

o3d::Bool MasterScene::mouseDoubleClickEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // keep pointer position
    m_pointerPos.set(event.localPos().x(), event.localPos().y(), 0);

    if (event.button(Mouse::LEFT)) {
        // double click on camera manipulator reset the rotation
        m_camera.get()->getNode()->getTransform()->setRotation(Quaternion());
    } else if (event.button(Mouse::MIDDLE)) {
        // nothing for now
    } else if (event.button(Mouse::RIGHT)) {
        // nothing for now
    }

    return True;
}

o3d::Bool MasterScene::mouseMoveEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // keep pointer position
    m_pointerPos.set(event.localPos().x(), event.localPos().y(), 0);

    Float elapsed = 0.01;//m_scene->getFrameManager()->getFrameDuration();
    Int32 deltaX = event.globalPos().x() - m_lockedPos.x();
    Int32 deltaY = event.globalPos().y() - m_lockedPos.y();

    if (m_lockedPos.x() < 0) {
        deltaX = 0;
    }

    if (m_lockedPos.y() < 0) {
        deltaY = 0;
    }

    // @todo use of the mouse smoother

    if (m_actionMode == ACTION_CAMERA_ZOOM) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float x = 0.f, y = 0.f, z = 0.f;

            x = deltaX * 100.f * elapsed;
            z = deltaY * 100.f * elapsed;

            if (o3d::abs(x) > o3d::abs(z)) {
                z = 0;
            } else {
                x = 0;
            }

            // slow motion if shift is down
            if (m_motionType == MOTION_PRECISE) {
                x *= 0.1;
                z *= 0.1;
            } else if (m_motionType == MOTION_FAST) {
                x *= 10;
                z *= 10;
            }

            /*
            // change axis if modifier
            if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
                x = z;
                z = 0;
            } else if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
                y = z;
                z = 0;
            }*/

            cameraNode->getTransform()->translate(Vector3(x, y, z));
        }
    } else if (m_actionMode == ACTION_CAMERA_ROTATION) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float speed = 1;

            // slow rotation if shift is down
            if (m_motionType == MOTION_PRECISE) {
                speed = 0.1;
            } else if (m_motionType == MOTION_FAST) {
                speed = 10;
            }

            cameraNode->getTransform()->rotate(Y, -deltaX * elapsed * speed);
            cameraNode->getTransform()->rotate(X, -deltaY * elapsed * speed);
        }
    } else if (m_actionMode == ACTION_CAMERA_TRANSLATION) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float x = 0.f, y = 0.f, z = 0.f;

            x = -deltaX * 100.f * elapsed;
            y = deltaY * 100.f * elapsed;

            // slow motion if shift is down
            if (m_motionType == MOTION_PRECISE) {
                x *= 0.1;
                y *= 0.1;
            } else if (m_motionType == MOTION_FAST) {
                x *= 10;
                y *= 10;
            }

            cameraNode->getTransform()->translate(Vector3(x, y, z));
        }
    } else {
        // we have a current manipulator
        if (m_hubManipulator && m_hubManipulator->isTransform()) {
            Float x = 0.f, y = 0.f, z = 0.f;

            x = deltaX * 1.f;  // * elapsed;
            y = deltaY * 1.f;  // * elapsed;

            // action using the manipulator
            m_hubManipulator->transform(Vector3(x, y, z), this);
        }
    }

    if (m_actionMode == ACTION_CAMERA_ROTATION ||
            m_actionMode == ACTION_CAMERA_TRANSLATION ||
            m_actionMode == ACTION_CAMERA_ZOOM) {
        // lock mouse position, infinite cursor
        QCursor cursor = m_content->cursor();
        QPoint pos(m_lockedPos.x(), m_lockedPos.y());

        cursor.setPos(pos);
        m_content->setCursor(cursor);
    } else if (m_actionMode == ACTION_TRANSFORM) {
        // lock mouse position, infinite cursor
        QCursor cursor = m_content->cursor();
        QPoint pos(m_lockedPos.x(), m_lockedPos.y());

        cursor.setPos(pos);
        m_content->setCursor(cursor);

        // comment for infinite mouse
        // m_lockedPos.set(event.globalPos().x(), event.globalPos().y());
    }

    // necessary at each move
    postPicking(m_pointerPos);

    return True;
}

o3d::Bool MasterScene::wheelEvent(const WheelEvent &event)
{
    if (!m_scene) {
        return False;
    }

    Float elapsed = m_scene->getFrameManager()->getFrameDuration();

    Int32 deltaX = event.angleDelta().x();   // ALT + or second wheel axis
    Int32 deltaY = event.angleDelta().y();

    // camera zoom/rotate
    if (deltaY != 0 || deltaX != 0) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
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
    }

    // could be necessary
    postPicking(m_pointerPos);

    return False;
}

o3d::Bool MasterScene::keyPressEvent(const KeyEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // motion modifier if action
    if (event.modifiers() & InputEvent::SHIFT_MODIFIER && event.modifiers() & InputEvent::ALT_MODIFIER) {
        m_motionType = MOTION_FAST;
    } else if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
        m_motionType = MOTION_PRECISE;
    } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
        m_motionType = MOTION_STEP;
    } else if (event.modifiers() & InputEvent::META_MODIFIER) {
        m_motionType = MOTION_MAGNET;
    } else if (event.modifiers() & InputEvent::ALT_MODIFIER) {
        m_motionType = MOTION_GRID;
    } else {
        m_motionType = MOTION_FOLLOW;
    }

    // switch action mode if no motifiers
    if (event.modifiers() == 0) {
        if (event.vKey() == o3d::VKey::KEY_ESCAPE) {
            if (m_hubManipulator && m_hubManipulator->isTransform()) {
                // cancel current transformation
                m_hubManipulator->cancelTransform(this);
            }
        }
    }

    if (m_hubManipulator && !m_hubManipulator->isTransform()) {
        // don't change during a transform
        m_hubManipulator->keyDownEvent(event, this);
    }

    // could be necessary
    postPicking(m_pointerPos);

    return True;
}

o3d::Bool MasterScene::keyReleaseEvent(const KeyEvent &event)
{
    if (!m_scene) {
        return False;
    }

    // motion modifier if action
    if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
        m_motionType = MOTION_PRECISE;
    } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
        m_motionType = MOTION_STEP;
    } else if (event.modifiers() & InputEvent::META_MODIFIER) {
        m_motionType = MOTION_MAGNET;
    } else if (event.modifiers() & InputEvent::ALT_MODIFIER) {
        m_motionType = MOTION_GRID;
    } else {
        m_motionType = MOTION_FOLLOW;
    }

    // could be necessary
    postPicking(m_pointerPos);

    return True;
}

o3d::Bool MasterScene::focusInEvent(const Event &/*event*/)
{
    m_motionType = MOTION_FOLLOW;
    setActionMode(ACTION_NONE);

    return False;
}

o3d::Bool MasterScene::focusOutEvent(const Event &/*event*/)
{
    // we don't want that focus out loose current displaying of a manipulator
    // @todo so is this ok and for focus in ?
    m_motionType = MOTION_FOLLOW;
    setActionMode(ACTION_NONE);

    return False;
}

o3d::Bool MasterScene::enterEvent(const Event &/*event*/)
{
    return False;
}

o3d::Bool MasterScene::leaveEvent(const Event &/*event*/)
{
    return False;
}

void MasterScene::updateCounters(o3d::UInt32 pass)
{
    if (m_scene && pass < 4) {
        m_verticesCount[pass] = m_scene->getFrameManager()->getCurrentNumVertices();
        m_trianglesCount[pass] = m_scene->getFrameManager()->getCurrentNumTriangles();
        m_linesCount[pass] = m_scene->getFrameManager()->getCurrentNumLines();
        m_pointCount[pass] = m_scene->getFrameManager()->getCurrentNumPoints();

//        if (pass > 0) {
//            m_verticesCount[pass] -= m_verticesCount[pass-1];
//            m_trianglesCount[pass] -= m_trianglesCount[pass-1];
//            m_linesCount[pass] -= m_linesCount[pass-1];
//            m_pointCount[pass] -= m_pointCount[pass-1];
//        }
    }
}

o3d::UInt32 MasterScene::numVertices(UInt32 pass) const
{
    if (pass < 4) {
        return m_verticesCount[pass] - (pass >= 1 ? m_verticesCount[pass-1] : 0);
    }
    return 0;
}

o3d::UInt32 MasterScene::numTriangles(UInt32 pass) const
{
    if (pass < 4) {
        return m_trianglesCount[pass] - (pass >= 1 ? m_trianglesCount[pass-1] : 0);
    }
    return 0;
}

o3d::UInt32 MasterScene::numLines(UInt32 pass) const
{
    if (pass < 4) {
        return m_linesCount[pass] - (pass >= 1 ? m_linesCount[pass-1] : 0);
    }
    return 0;
}

o3d::UInt32 MasterScene::numPoints(UInt32 pass) const
{
    if (pass < 4) {
        return m_pointCount[pass] - (pass >= 1 ? m_pointCount[pass-1] : 0);
    }
    return 0;
}

MasterScene::ActionMode MasterScene::actionMode() const
{
    return m_actionMode;
}

MasterScene::MotionType MasterScene::motionType() const
{
    return m_motionType;
}

o3d::Int32 MasterScene::transformMode() const
{
    return m_transformMode;
}

void MasterScene::registerPickingId(o3d::UInt32 id, SceneUIElement *element)
{
    if (!element) {
        return;
    }

    auto it = m_pickingToSceneUIElements.find(id);
    if (it != m_pickingToSceneUIElements.end()) {
        O3D_ERROR(E_InvalidOperation("Picking id already registered"));
    }

    m_pickingToSceneUIElements[id] = element;
}

void MasterScene::unregisterPickingId(o3d::UInt32 id)
{
    auto it = m_pickingToSceneUIElements.find(id);
    if (it != m_pickingToSceneUIElements.end()) {
        m_pickingToSceneUIElements.erase(it);
    }
}

void MasterScene::pickingNoHit()
{
    if (m_hoverHub) {
        // @todo a leave hub
        m_hoverHub = nullptr;
    }

    if (m_hoverUIElement) {
        m_hoverUIElement->leave();
        m_hoverUIElement = nullptr;
    }
}

void MasterScene::pickingHit(Pickable *pickable, Vector3 pos)
{
    m_hoverHub = nullptr;

    SceneObject *sceneObject = o3d::dynamicCast<SceneObject*>(pickable);
    if (sceneObject) {
        // o3d object to o3s hub
        m_hoverHub = project()->lookupPickable(pickable->getPickableId());
        m_pickPos = pos;
    }

    if (m_hoverUIElement) {
        // but no hover ui element
        m_hoverUIElement->leave();
        m_hoverUIElement = nullptr;
    }
}

void MasterScene::elementPickingHit(o3d::UInt32 id, o3d::Vector3 pos)
{
    m_hoverUIElement = nullptr;

    auto it = m_pickingToSceneUIElements.find(id);
    if (it != m_pickingToSceneUIElements.end()) {
        it->second->hover(id, pos);
        m_hoverUIElement = it->second;
    }

    if (m_hoverHub) {
        // but no hover hub
        // @todo a leave hub
        m_hoverHub = nullptr;
    }
}

void MasterScene::onSelectionChanged()
{
    const std::set<common::SelectionItem *> currentSelection =
            common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::hub());

    common::Hub *hub = nullptr;

    // cleanup current selection
    if (m_hubManipulator) {
        m_hubManipulator->setSelection(this, std::list<StructuralHub*>());
    }

    std::list<StructuralHub*> hubs;

    if (Application::instance()->selection().acceptedRole() == Selection::ACCEPT_STRUCTURAL_HUB) {

    } else if (Application::instance()->selection().acceptedRole() == Selection::ACCEPT_PROPERTY_HUB) {

    }

    for (common::SelectionItem *selectionItem : currentSelection) {
        // only for related project
        if (selectionItem->ref().projectId() != project()->ref().light().id()) {
            continue;
        }

        if (selectionItem->ref().baseTypeOf(TypeRef::hub())) {
            hub = static_cast<Hub*>(project()->lookup(selectionItem->ref()));

            if (hub->role() != Entity::ROLE_STRUCTURAL_HUB) {
                // accept only structure hubs
                continue;
            }

            if (hub->parent() == hub->project()) {
                // root hub cannot be manipulated
                continue;
            }

            hubs.push_back(static_cast<StructuralHub*>(hub));
        }
    }

    if (hubs.size() && m_hubManipulator) {
        // create the new according to the current builder (@todo not for now only the standard manipulator)
        m_hubManipulator->setSelection(this, hubs);
    } else {
        // or no action if no selection
        setActionMode(ACTION_NONE);
    }
}

void MasterScene::changeTransformMode(o3d::Int32 mode)
{
    m_transformMode = mode;

    if (m_hubManipulator) {
        m_hubManipulator->setTransforMode((HubManipulator::TransformMode)mode);
    }
}

void MasterScene::changeTransformOrientation(o3d::Int32 mode)
{
    m_orientationMode = mode;

    if (m_hubManipulator) {
        m_hubManipulator->setTransformOrientation(HubManipulator::TransformOrientation(mode));
    }
}

void MasterScene::changePivotMode(o3d::Int32 mode)
{
    m_pivotMode = mode;

    if (m_hubManipulator) {
        m_hubManipulator->setPivotPoint(HubManipulator::PivotPoint(mode));
    }
}

void MasterScene::setActionMode(MasterScene::ActionMode actionMode)
{
    m_prevActionMode = m_actionMode;
    m_actionMode = actionMode;
}

void MasterScene::processCommands()
{
    // process commands (FIFO)
    SceneCommand *command = nullptr;
    while (m_commands.size() > 0) {
        command = m_commands.front();
        m_commands.pop_front();

        if (m_scene) {
            command->process(this);
        }

        delete command;
    }
}

void MasterScene::postPicking(const Vector3f &position)
{
    // only at once (and could add a small timer to avoid a lot of events)
    if (!m_scene->getPicking()->isPickingToProcess() && !m_scene->getPicking()->isProcessingPicking()) {
        // picking or manipulation of a transformer
        m_scene->getPicking()->postPickingEvent(
                    (UInt32)position.x(),
                    m_scene->getViewPortManager()->getReshapeHeight() - (UInt32)position.y());
    }
}

Entity* MasterScene::findSelectable(Entity *entity) const
{
    if (entity && entity->ref().light().baseTypeOf(TypeRef::hub())) {
        Hub *hub = static_cast<Hub*>(entity);

        if (Application::instance()->selection().acceptedRole() == Selection::ACCEPT_STRUCTURAL_HUB) {
            if (hub->role() != Entity::ROLE_STRUCTURAL_HUB) {
                // @todo select parent if structural hub

                while (hub->isParentHub()) {
                    hub = static_cast<Hub*>(hub->parent());
                    if (hub->role() == Entity::ROLE_STRUCTURAL_HUB) {
                        return hub;
                    }
                }

                return nullptr;
            }
        } else if (Application::instance()->selection().acceptedRole() == Selection::ACCEPT_PROPERTY_HUB) {
            if (hub->role() != Entity::ROLE_HUB) {
                // @todo select first child hub

                return nullptr;
            }
        }

        return hub;
    }

    return nullptr;
}

void MasterScene::initializeDrawer()
{
    if (!m_scene && m_renderer && m_parent && project()) {
        // once GL context is ready
        LocalDir path(project()->path());
        if (!path.isAbsolute()) {
            path.makeAbsolute();
        }

        m_scene = new o3d::Scene(nullptr, path.getFullPathName(), m_renderer);

        // default all symbolics objects are drawn
        m_scene->drawAllSymbolicObject();

        // a typical blue background
        m_scene->getContext()->setBackgroundColor(0.633f, 0.792f, 0.914f, 0.0f);
        m_scene->setGlobalAmbient(Color(0.5f, 0.5f, 0.5f));

        // visibility manager (default to simple)
        // @todo current quadtree is not adapted to edition have an octree or quadtree
        // more specialized for edition
        m_scene->getVisibilityManager()->setGlobal(VisibilityManager::DISTANCE, 8, 128);
        m_scene->getVisibilityManager()->disableMaxDistance();

        // don't display visibility manager debug data
        m_scene->setDrawObject(Scene::DRAW_VISIBILITY, False);

        // working camera @todo might be configurable per project
        m_camera = new o3d::Camera(m_scene);
        m_camera.get()->setZnear(0.25f);
        m_camera.get()->setZfar(10000.f);
        m_camera.get()->setFov(60.f/*45.f*/);
        m_camera.get()->disableVisibility();   // never visible

        Node *cameraNode = m_scene->getHierarchyTree()->addNode(m_camera.get());
        cameraNode->addTransform(new STransform());

        // working drawer and viewport
        m_sceneDrawer = new MasterSceneDrawer(m_scene, this);
        m_viewport = m_scene->getViewPortManager()->addScreenViewPort(m_camera.get(), m_sceneDrawer.get(), 0);

        // enable picking by color and connect its signal
        m_scene->getPicking()->setMode(Picking::COLOR);
        m_scene->getPicking()->onHit.connect(this, &MasterScene::pickingHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->onUnknownHit.connect(this, &MasterScene::elementPickingHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->onNoHit.connect(this, &MasterScene::pickingNoHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->setCamera(m_camera.get());

        // add an helper grid @todo configurable per project
        Int32 gridStep = (Int32)(200.f * m_camera.get()->getZnear());
        Float size = o3d::min(m_camera.get()->getZfar() * 0.8f, 1000.f);
        Int32 gridHW = (Int32)(size);

        Grid *grid = new Grid(this, Point3f(), Point2i(gridHW, gridHW), Point2i(gridStep, gridStep));
        addSceneUIElement(grid);

        // add a HUD for common info about current scene
        TrueTypeFont::initializeFreeType();

        // @todo position and visibility configurable globally and or override per project
        TrueTypeFont *font2d = new TrueTypeFont(m_scene);
        font2d->load("rc/fonts/arial.ttf", TrueTypeFont::CHARSET_LATIN1);
        InfoHUD *infoHUD = new InfoHUD(this, Point2f(0.02f, 0.02f), font2d);
        addSceneUIElement(infoHUD);

        // a camera helper
        m_cameraManipulator = new CameraManipulator(this, Point2f(0.9f, 0.1f), 1.f);
        addSceneUIElement(m_cameraManipulator);

        // and a default selection hub manipulator
        m_hubManipulator = new HubManipulator(this);
        addSceneUIElement(m_hubManipulator);

        common::UiController &uiCtrl = common::Application::instance()->ui();
        common::TransformToolBar *transformToolBar = static_cast<TransformToolBar*>(uiCtrl.toolBar("o3s::main::toolbar::transform"));

        transformToolBar->onChangeTransformMode.connect(this, &MasterScene::changeTransformMode);
        transformToolBar->onChangeOrientationMode.connect(this, &MasterScene::changeTransformOrientation);
        transformToolBar->onChangePivotMode.connect(this, &MasterScene::changePivotMode);
    }
}

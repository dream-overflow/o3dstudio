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

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"
#include "o3d/studio/common/ui/scene/hubmanipulator.h"

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
    m_motionType(MOTION_FOLLOW),
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

const o3d::Transform& MasterScene::cameraTransform() const
{
    return *m_camera.get()->getNode()->getTransform();
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
    // process commands (FIFO)
    processCommands();

    if (m_scene) {
        // CPU and GPU, need GL context
        m_scene->display();
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
       //     m_sceneDrawer.get()->removeSceneUIElement(elt);

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

    if (event.button(Mouse::LEFT)) {
        if (m_actionMode == ACTION_NONE && m_hubManipulator) {
            // default to translation
            m_actionMode = ACTION_TRANSLATION;
        }

        // action using the manipulator
        if (m_hoverUIElement && m_hubManipulator == m_hoverUIElement) {
            m_hubManipulator->beginTransform(this);
        } else {
            m_actionMode = ACTION_SELECTION;

            if (m_hoverHub) {
                if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
                    if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
                        // get previous selection and remove
                        auto previous = Application::instance()->selection().filterCurrentByBaseType(TypeRef::hub());

                        Application::instance()->selection().beginSelection();

                        for (SelectionItem *item : previous) {
                            if (item->ref() != m_hoverHub->ref().light()) {
                                Application::instance()->selection().appendSelection(
                                            static_cast<Hub*>(project()->lookup(item->ref())));
                            }
                        }

                        Application::instance()->selection().appendSelection(m_hoverHub);
                        Application::instance()->selection().endSelection();

                        // an object is removed from selection
                        Application::instance()->messenger().message(Messenger::DEBUG_MSG, String("Remove from selection on {0} at {1}").arg(m_hoverHub->name()).arg(m_pickPos));
                    } else {
                        // get previous selection and append
                        auto previous = Application::instance()->selection().filterCurrentByBaseType(TypeRef::hub());

                        Application::instance()->selection().beginSelection();

                        for (SelectionItem *item : previous) {
                            // himself is reselected at last
                            if (item->ref() != m_hoverHub->ref().light()) {
                                Application::instance()->selection().appendSelection(
                                            static_cast<Hub*>(project()->lookup(item->ref())));
                            }
                        }

                        Application::instance()->selection().appendSelection(m_hoverHub);
                        Application::instance()->selection().endSelection();

                        // an object is selected
                        Application::instance()->messenger().message(Messenger::DEBUG_MSG, String("Multiple selection on {0} at {1}").arg(m_hoverHub->name()).arg(m_pickPos));
                    }
                }
            }
        }
    } else if (event.button(Mouse::MIDDLE)) {
        if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
            m_actionMode = ACTION_CAMERA_TRANSLATION;
            m_motionType = MOTION_FOLLOW;  // default normal speed, press again shift to slow
        } else if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
            m_actionMode = ACTION_CAMERA_ZOOM;
        } else {
            m_actionMode = ACTION_CAMERA_ROTATION;
        }
    } else if (event.button(Mouse::RIGHT)) {

    }

    if (m_actionMode == ACTION_CAMERA_ROTATION || m_actionMode == ACTION_CAMERA_TRANSLATION) {
        // no cursor and infinite scrolling
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::BlankCursor);

        m_content->setCursor(cursor);
        m_lockedPos = event.globalPos();
    } else if (m_actionMode == ACTION_ROTATION || m_actionMode == ACTION_TRANSLATION ||
               m_actionMode == ACTION_SCALE || m_actionMode == ACTION_SKEW) {

        // initial relative
        m_lockedPos = event.globalPos();
    }

    return True;
}

o3d::Bool MasterScene::mouseReleaseEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    if (event.button(Mouse::LEFT)) {
        if (m_actionMode == ACTION_SELECTION) {
            if (m_hoverHub) {
                // initial selection
                Application::instance()->selection().select(m_hoverHub);

                // an object is selected
                Application::instance()->messenger().message(Messenger::DEBUG_MSG, String("Initial selection on {0} at {1}").arg(m_hoverHub->name()).arg(m_pickPos));
            } else {
                // clear selection
                Application::instance()->selection().unselectAll();
            }
        } else if (m_hubManipulator && m_hubManipulator->hasFocus()) {
            // action using the manipulator
            HubManipulator *hubManipulator = static_cast<HubManipulator*>(m_hubManipulator);
            hubManipulator->endTransform();
        }
    } else if (event.button(Mouse::MIDDLE)) {
        if (m_actionMode == ACTION_CAMERA_ROTATION ||
            m_actionMode == ACTION_CAMERA_TRANSLATION ||
            m_actionMode == ACTION_CAMERA_ZOOM) {
            // stop camera action
            m_actionMode = ACTION_NONE;
        }
    } else if (event.button(Mouse::RIGHT)) {
    }

    if (m_actionMode == ACTION_NONE && m_hubManipulator) {
        // default to translation
        m_actionMode = ACTION_TRANSLATION;
    }

    if (m_actionMode != ACTION_CAMERA_ROTATION && m_actionMode != ACTION_CAMERA_TRANSLATION) {
        // restore cursor
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::ArrowCursor);

        m_content->setCursor(cursor);
    }

    return True;
}

o3d::Bool MasterScene::mouseDoubleClickEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    if (event.button(Mouse::LEFT)) {

    } else if (event.button(Mouse::MIDDLE)) {
        // @todo double click on camera manipulator reset the rotation
        m_camera.get()->getNode()->getTransform()->setRotation(Quaternion());
    } else if (event.button(Mouse::RIGHT)) {

    }

    return True;
}

o3d::Bool MasterScene::mouseMoveEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    Float elapsed = m_scene->getFrameManager()->getFrameDuration();
    Int32 deltaX = event.globalPos().x() - m_lockedPos.x();
    Int32 deltaY = event.globalPos().y() - m_lockedPos.y();

    if (m_lockedPos.x() < 0) {
        deltaX = 0;
    }

    if (m_lockedPos.y() < 0) {
        deltaY = 0;
    }

    if (m_actionMode == ACTION_CAMERA_ZOOM) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float x = 0.f, y = 0.f, z = 0.f;

            z = deltaY * 100.f * elapsed;

            // change axis if modifier
            if (event.modifiers() & InputEvent::CTRL_MODIFIER) {
                x = z;
                z = 0;
            } else if (event.modifiers() & InputEvent::SHIFT_MODIFIER) {
                y = z;
                z = 0;
            }

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

            // @todo use of mouse smoother
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
        if (m_hubManipulator && m_hubManipulator->hasFocus()) {
            Float x = 0.f, y = 0.f, z = 0.f;

            x = deltaX * 1.f;   // * elapsed;
            y = -deltaY * 1.f;  // * elapsed;
            z = deltaY * 1.f;   // * elapsed;

            // action using the manipulator
            m_hubManipulator->transform(Vector3(x, y, z), this);
        }

        // only at once (and could add a small timer to avoid a lot of events)
        if (!m_scene->getPicking()->isPickingToProcess() && !m_scene->getPicking()->isProcessingPicking()) {
            // picking or manipulation of a transformer
            m_scene->getPicking()->postPickingEvent(
                        (UInt32)event.localPos().x(),
                        m_scene->getViewPortManager()->getReshapeHeight() - (UInt32)event.localPos().y());
        }
    }

    if (m_actionMode == ACTION_CAMERA_ROTATION || m_actionMode == ACTION_CAMERA_TRANSLATION) {
        // lock mouse position, infinite cursor
        QCursor cursor = m_content->cursor();
        QPoint pos(m_lockedPos.x(), m_lockedPos.y());

        cursor.setPos(pos);
        m_content->setCursor(cursor);
    } else if (m_actionMode == ACTION_ROTATION || m_actionMode == ACTION_TRANSLATION ||
               m_actionMode == ACTION_SCALE || m_actionMode == ACTION_SKEW) {
        // relative
        m_lockedPos.set(event.globalPos().x(), event.globalPos().y());
    }

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
        if (event.vKey() == o3d::VKey::KEY_R) {
            m_actionMode = ACTION_ROTATION;
        } else if (event.vKey() == o3d::VKey::KEY_S) {
            m_actionMode = ACTION_SCALE;
        } else if (event.vKey() == o3d::VKey::KEY_T) {
            m_actionMode = ACTION_TRANSLATION;
        } else if (event.vKey() == o3d::VKey::KEY_G) {
            m_actionMode = ACTION_SKEW;
        } else if (event.vKey() == o3d::VKey::KEY_ESCAPE) {
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

    return True;
}

o3d::Bool MasterScene::focusInEvent(const Event &/*event*/)
{
    m_motionType = MOTION_FOLLOW;
    m_actionMode = ACTION_NONE;

    return False;
}

o3d::Bool MasterScene::focusOutEvent(const Event &/*event*/)
{
    m_motionType = MOTION_FOLLOW;
    m_actionMode = ACTION_NONE;

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
}

void MasterScene::elementPickingHit(o3d::UInt32 id, o3d::Vector3 pos)
{
    m_hoverUIElement = nullptr;

    auto it = m_pickingToSceneUIElements.find(id);
    if (it != m_pickingToSceneUIElements.end()) {
        it->second->hover(id, pos);
        m_hoverUIElement = it->second;
    }
}

void MasterScene::onSelectionChanged()
{
    const std::set<common::SelectionItem *> currentSelection =
            common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::hub());

    common::Hub *hub = nullptr;

    // delete a potentiel previous instance of manipulator
    if (m_hubManipulator) {
        removeSceneUIElement(m_hubManipulator);
       // deletePtr(m_hubManipulator);
       m_hubManipulator = nullptr; // @todo why double free corruption ?
    }

    std::list<Hub*> hubs;

    for (common::SelectionItem *selectionItem : currentSelection) {
        // only for related project
        if (selectionItem->ref().projectId() != project()->ref().light().id()) {
            continue;
        }

        if (selectionItem->ref().baseTypeOf(TypeRef::hub())) {
            hub = static_cast<Hub*>(project()->lookup(selectionItem->ref()));
            hubs.push_back(hub);
        }
    }

    if (hubs.size()) {
        // create the new according to the current builder (@todo not for now only the standard manipulator)
        m_hubManipulator = new HubManipulator(this, hubs);
        addSceneUIElement(m_hubManipulator);

        // default to translation when selection
        m_actionMode = ACTION_TRANSLATION;
    } else {
        // or no action if no selection
        m_actionMode = ACTION_NONE;
    }
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

void MasterScene::initializeDrawer()
{
    if (!m_scene && m_renderer && m_parent && project()) {
        // once GL context is ready
        LocalDir path(project()->path());
        if (!path.isAbsolute()) {
            path.makeAbsolute();
        }

        m_scene = new o3d::Scene(nullptr, path.getFullPathName(), m_renderer);

        m_scene->getContext()->setBackgroundColor(0.633f, 0.792f, 0.914f, 0.0f);
        m_scene->setGlobalAmbient(Color(0.5f, 0.5f, 0.5f));

        // working camera
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

        // default all symbolics objects are drawn
        m_scene->drawAllSymbolicObject();

        // enable picking by color and connect its signal
        m_scene->getPicking()->setMode(Picking::COLOR);
        m_scene->getPicking()->onHit.connect(this, &MasterScene::pickingHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->onUnknownHit.connect(this, &MasterScene::elementPickingHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->onNoHit.connect(this, &MasterScene::pickingNoHit, EvtHandler::CONNECTION_ASYNCH);
        m_scene->getPicking()->setCamera(m_camera.get());

        // add an helper grid
        Int32 gridStep = (Int32)(200.f * m_camera.get()->getZnear());
        Float size = o3d::min(m_camera.get()->getZfar() * 0.8f, 1000.f);
        Int32 gridHW = (Int32)(size);

        Grid *grid = new Grid(this, Point3f(), Point2i(gridHW, gridHW), Point2i(gridStep, gridStep));
        addSceneUIElement(grid);

        // add a HUD for common info about current scene
        TrueTypeFont::initializeFreeType();

        TrueTypeFont *font2d = new TrueTypeFont(m_scene);
        font2d->load("rc/fonts/arial.ttf", TrueTypeFont::CHARSET_LATIN1);
        InfoHUD *infoHUD = new InfoHUD(this, Point2f(0.02f, 0.02f), font2d);
        addSceneUIElement(infoHUD);

        // and a camera helper
        m_cameraManipulator = new CameraManipulator(this, Point2f(0.9f, 0.1f), 1.f);
        addSceneUIElement(m_cameraManipulator);
    }
}

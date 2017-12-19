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

#include <o3d/engine/glextdefines.h>
#include <o3d/engine/glextensionmanager.h>

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/viewportmanager.h>
#include <o3d/engine/screenviewport.h>
#include <o3d/engine/shadow/shadowvolumeforward.h>
#include <o3d/engine/hierarchy/hierarchytree.h>
#include <o3d/engine/utils/framemanager.h>
#include <o3d/engine/object/ftransform.h>
#include <o3d/engine/context.h>
#include <o3d/engine/picking.h>

#include <o3d/gui/gui.h>
#include <o3d/gui/fontmanager.h>
#include <o3d/gui/truetypefont.h>

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include "o3d/studio/common/ui/scene/grid.h"
#include "o3d/studio/common/ui/scene/infohud.h"
#include "o3d/studio/common/ui/scene/cameramanipulator.h"

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
    m_rotateCam(False),
    m_moveCam(False),
    m_camera(this),
    m_viewport(this),
    m_sceneDrawer(this)
{
    O3D_ASSERT(m_parent != nullptr);
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

void MasterScene::addCommand(SceneCommand *command)
{
    if (command) {
        m_commands.push_front(command);

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

    m_sceneUIElements.push_back(elt);

    // add to the scene drawer to be displayed
    m_sceneDrawer.get()->addSceneUIElement(elt);
}

void MasterScene::removeSceneUIElement(SceneUIElement *elt)
{
    auto it = std::find(m_sceneUIElements.begin(), m_sceneUIElements.end(), elt);
    if (it != m_sceneUIElements.end()) {
        // remove from the scene drawer if initialized
        if (m_sceneDrawer.isValid()) {
            m_sceneDrawer.get()->removeSceneUIElement(*it);
        }

        m_sceneUIElements.erase(it);
    }
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
        m_rotateCam = True;
    } else if (event.button(Mouse::RIGHT)) {
        m_moveCam = True;
    }

    if (m_rotateCam || m_moveCam) {
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::BlankCursor);

        m_content->setCursor(cursor);
        m_lockedPos = event.globalPos();
    }

    return m_rotateCam || m_moveCam;
}

o3d::Bool MasterScene::mouseReleaseEvent(const MouseEvent &event)
{
    if (!m_scene) {
        return False;
    }

    if (event.button(Mouse::LEFT)) {
        m_rotateCam = False;
    } else if (event.button(Mouse::RIGHT)) {
        m_moveCam = False;
    }

    if (!m_rotateCam && !m_moveCam) {
        QCursor cursor = m_content->cursor();
        cursor.setShape(Qt::ArrowCursor);

        m_content->setCursor(cursor);
    }

    return m_rotateCam || m_moveCam;
}

o3d::Bool MasterScene::mouseDoubleClickEvent(const MouseEvent &/*event*/)
{
    return False;
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

    if (m_rotateCam && m_moveCam) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float z = 0.f;

            z = deltaY * 100.f * elapsed;

            cameraNode->getTransform()->translate(Vector3(0.f, 0.f, z));
        }
    } else if (m_rotateCam) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            cameraNode->getTransform()->rotate(Y, -deltaX * elapsed);
            cameraNode->getTransform()->rotate(X, -deltaY * elapsed);
        }
    } else if (m_moveCam) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float x = 0.f, y = 0.f, z = 0.f;

            x = deltaX * 100.f * elapsed;
            y = -deltaY * 100.f * elapsed;

            cameraNode->getTransform()->translate(Vector3(x, y, z));
        }
    }

    if (m_rotateCam || m_moveCam) {
        // lock mouse position
        QCursor cursor = m_content->cursor();
        QPoint pos(m_lockedPos.x(), m_lockedPos.y());

        cursor.setPos(pos);
        m_content->setCursor(cursor);
    }

    return m_rotateCam || m_moveCam;
}

o3d::Bool MasterScene::wheelEvent(const WheelEvent &event)
{
    if (!m_scene) {
        return False;
    }

    Float elapsed = m_scene->getFrameManager()->getFrameDuration();
    // Int32 deltaX = event.angleDelta().x();  // too rarely supported
    Int32 deltaY = event.angleDelta().y();

    if (deltaY != 0) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            Float z = 0.f;

            z = -deltaY * 100.f / 120.f * 10.f * elapsed;

            cameraNode->getTransform()->translate(Vector3(0.f, 0.f, z));

            return True;
        }
    }

    return False;
}

o3d::Bool MasterScene::keyPressEvent(const KeyEvent &/*event*/)
{
    return False;
}

o3d::Bool MasterScene::keyReleaseEvent(const KeyEvent &/*event*/)
{
    return False;
}

o3d::Bool MasterScene::focusInEvent(const Event &/*event*/)
{
    return False;
}

o3d::Bool MasterScene::focusOutEvent(const Event &/*event*/)
{
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
        m_verticesCount[pass] = m_scene->getFrameManager()->getNumVertices();
        m_trianglesCount[pass] = m_scene->getFrameManager()->getNumTriangles();
        m_linesCount[pass] = m_scene->getFrameManager()->getNumLines();
        m_pointCount[pass] = m_scene->getFrameManager()->getNumPoints();

        if (pass > 0) {
            m_verticesCount[pass] -= m_verticesCount[pass-1];
            m_trianglesCount[pass] -= m_trianglesCount[pass-1];
            m_linesCount[pass] -= m_linesCount[pass-1];
            m_pointCount[pass] -= m_pointCount[pass-1];
        }
    }
}

o3d::UInt32 MasterScene::numVertices(UInt32 pass) const
{
    if (pass < 4) {
        return m_verticesCount[pass];
    }
    return 0;
}

o3d::UInt32 MasterScene::numTriangles(UInt32 pass) const
{
    if (pass < 4) {
        return m_trianglesCount[pass];
    }
    return 0;
}

o3d::UInt32 MasterScene::numLines(UInt32 pass) const
{
    if (pass < 4) {
        return m_linesCount[pass];
    }
    return 0;
}

o3d::UInt32 MasterScene::numPoints(UInt32 pass) const
{
    if (pass < 4) {
        return m_pointCount[pass];
    }
    return 0;
}

void MasterScene::pickingHit(Pickable *pickable, Vector3 pos)
{
    // @todo
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

        m_scene->getContext()->setBackgroundColor(0.633f,0.792f,.914f,0.0f);
        // m_scene->setGlobalAmbient();

        // working camera
        m_camera = new o3d::Camera(m_scene);
        m_camera.get()->setZnear(0.25f);
        m_camera.get()->setZfar(10000.f);
        m_camera.get()->disableVisibility();   // never visible

        Node *cameraNode = m_scene->getHierarchyTree()->addNode(m_camera.get());
        cameraNode->addTransform(new FTransform());

        // working drawer and viewport
        m_sceneDrawer = new MasterSceneDrawer(m_scene, this);
        m_viewport = m_scene->getViewPortManager()->addScreenViewPort(m_camera.get(), m_sceneDrawer.get(), 0);

        // default all symbolics objects are drawn
        m_scene->drawAllSymbolicObject();

        // add an helper grid
        Int32 gridStep = (Int32)(200.f * m_camera.get()->getZnear());
        Float size = o3d::min(m_camera.get()->getZfar() * 0.8f, 1000.f);
        Int32 gridHW = (Int32)(size);

        Grid *grid = new Grid(this, Point3f(), Point2i(gridHW, gridHW), Point2i(gridStep, gridStep));
        addSceneUIElement(grid);

        // add a HUD for common info about current scene
        TrueTypeFont::initializeFreeType();  // @todo later done by using O3D GUI

        TrueTypeFont *font2d = new TrueTypeFont(m_scene);
        font2d->load("rc/fonts/arial.ttf", TrueTypeFont::CHARSET_LATIN1);
        InfoHUD *infoHUD = new InfoHUD(this, Point2f(0.02f, 0.02f), font2d);
        addSceneUIElement(infoHUD);

        // and a camera helper
        CameraManipulator *cameraManipulator = new CameraManipulator(this, Point2f(0.9f, 0.1f), 1.f);
        addSceneUIElement(cameraManipulator);

        // picking slot
        m_scene->getPicking()->setMode(Picking::COLOR);
        m_scene->getPicking()->onHit.connect(this, &MasterScene::pickingHit);
    }
}

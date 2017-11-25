/**
 * @brief Workspace master scene rendering and management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#include <o3d/engine/glextensionmanager.h>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/ui/qtrenderer.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/viewportmanager.h>
#include <o3d/engine/screenviewport.h>
#include <o3d/engine/shadow/shadowvolumeforward.h>
#include <o3d/engine/hierarchy/hierarchytree.h>
#include <o3d/engine/utils/framemanager.h>
#include <o3d/engine/object/ftransform.h>

#define __gl_h_
#define __glext_h_

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include "o3d/studio/common/ui/keyevent.h"
//#include "o3d/studio/common/ui/mouseevent.h"
//#include "o3d/studio/common/ui/focusevent.h"

using namespace o3d::studio::common;


MasterScene::MasterScene(Entity *parent) :
    BaseObject(nullptr),  // @todo may entity be baseobject ?
    m_parent(parent),
    m_content(nullptr),
    m_renderer(nullptr),
    m_scene(nullptr),
    m_rotateCam(False),
    m_camera(this),
    m_viewport(this),
    m_sceneDrawer(this)
{
    O3D_ASSERT(m_parent != nullptr);
}

MasterScene::~MasterScene()
{
    if (m_scene) {
        delete m_scene;
    }

    if (m_content) {
        common::UiController &uiCtrl = common::Application::instance()->ui();
        uiCtrl.removeContent(m_content);

        m_content->deleteLater();
    }

    if (m_renderer) {
        delete m_renderer;
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

QtRenderer *MasterScene::renderer()
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
    if (m_scene) {
        m_scene->display();
    }
}

void MasterScene::updateDrawer()
{
    if (m_scene) {
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
    if (m_content) {
        common::UiController &uiCtrl = common::Application::instance()->ui();
        uiCtrl.removeContent(m_content);

        m_content = nullptr;
    }

    if (m_scene) {
//        m_scene->getHierarchyTree()->getRootNode()->removeSon(*m_camera);
//        deletePtr(m_camera->getNode());

        m_camera = nullptr;
        m_viewport = nullptr;
        m_sceneDrawer = nullptr;

        delete m_scene;
        m_scene = nullptr;
    }

    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void MasterScene::mousePressEvent(const MouseEvent &event)
{
    if (event.button(Mouse::LEFT)) {
        m_rotateCam = True;
    }
}

void MasterScene::mouseReleaseEvent(const MouseEvent &event)
{
    if (event.button(Mouse::LEFT)) {
        m_rotateCam = False;
    }
}

void MasterScene::mouseDoubleClickEvent(const MouseEvent &event)
{

}

void MasterScene::mouseMoveEvent(const MouseEvent &event)
{
    Float elapsed = m_scene->getFrameManager()->getFrameDuration();
    Int32 deltaX = event.localPos().x() - m_localPos.x();
    Int32 deltaY = event.localPos().y() - m_localPos.y();

    // keep for next delta calculation
    m_localPos = event.localPos();

    if (m_localPos.x() < 0) {
        deltaX = 0;
    }

    if (m_localPos.y() < 0) {
        deltaY = 0;
    }

    if (m_rotateCam) {
        BaseNode *cameraNode = m_camera.get()->getNode();
        if (cameraNode) {
            cameraNode->getTransform()->rotate(Y, -deltaX * elapsed);
            cameraNode->getTransform()->rotate(X, -deltaY * elapsed);
        }
    }
}

void MasterScene::wheelEvent(const MouseEvent &event)
{

}

void MasterScene::keyPressEvent(const KeyEvent &event)
{

}

void MasterScene::keyReleaseEvent(const KeyEvent &event)
{

}

void MasterScene::focusInEvent(const Event &event)
{

}

void MasterScene::focusOutEvent(const Event &event)
{

}

void MasterScene::enterEvent(const Event &event)
{

}

void MasterScene::leaveEvent(const Event &event)
{

}

void MasterScene::initializeDrawer()
{
    if (!m_scene && m_renderer && m_parent && project()) {
        // once GL context is ready
        DiskDir path(project()->path());
        if (!path.isAbsolute()) {
            path.makeAbsolute();
        }

        m_scene = new o3d::Scene(nullptr, path.getFullPathName(), m_renderer);

        m_camera = new o3d::Camera(m_scene);
        Node *cameraNode = m_scene->getHierarchyTree()->addNode(m_camera.get());
        cameraNode->addTransform(new FTransform());

        m_sceneDrawer = new o3d::ShadowVolumeForward(m_scene);
        m_viewport = m_scene->getViewPortManager()->addScreenViewPort(m_camera.get(), m_sceneDrawer.get(), 0);

        m_localPos.set(-1, -1);
    }
}

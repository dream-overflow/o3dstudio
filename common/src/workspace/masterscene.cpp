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
#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"
#include "o3d/studio/common/ui/qtrenderer.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include <o3d/engine/scene/scene.h>

using namespace o3d::studio::common;


MasterScene::MasterScene(Entity *parent) :
    m_parent(parent),
    m_content(nullptr),
    m_renderer(nullptr),
    m_scene(nullptr)
{
    Q_ASSERT(m_parent != nullptr);
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

void MasterScene::initialize()
{
    if (m_content || m_scene || m_renderer) {
        return;
    }

    // master OpenGL canvas content
    // @todo debug options
    m_content = new common::O3DCanvasContent(project()->ref().light(), false);
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
        delete m_scene;
        m_scene = nullptr;
    }

    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void MasterScene::initializeDrawer()
{
    if (!m_scene && m_renderer && m_parent && project()) {
        // once GL context is ready
        m_scene = new o3d::Scene(nullptr, project()->path().absolutePath().toStdWString().c_str(), m_renderer);
    }
}

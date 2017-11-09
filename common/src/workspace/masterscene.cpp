/**
 * @brief Workspace master scene rendering and management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-01
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"
#include "o3d/studio/common/ui/qtrenderer.h"
#include "o3d/studio/common/ui/uicontroller.h"

#include <o3d/engine/scene/scene.h>

using namespace o3d::studio::common;


MasterScene::MasterScene(Project *project) :
    m_project(project),
    m_content(nullptr),
    m_renderer(nullptr),
    m_scene(nullptr)
{

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

const Project *MasterScene::project() const
{
    return m_project;
}

Project *MasterScene::project()
{
    return m_project;
}

QtRenderer *MasterScene::renderer()
{
    return m_renderer;
}

O3DCanvasContent* MasterScene::widget()
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
    m_content = new common::O3DCanvasContent(m_project->ref().light(), false);
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
    if (!m_scene && m_renderer && m_project) {
        // once GL context is ready
        m_scene = new o3d::Scene(nullptr, m_project->path().absolutePath().toStdWString().c_str(), m_renderer);
    }
}

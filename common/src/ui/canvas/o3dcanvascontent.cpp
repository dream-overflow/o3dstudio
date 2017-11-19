/**
 * @brief Common OpenGL 4.5 canvas content with O3D specialisation
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#include <QtCore/QTimer>

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

#include <o3d/engine/context.h>

using namespace o3d::studio::common;

O3DCanvasContent::O3DCanvasContent(const LightRef &ref, bool debug, QWidget *parent) :
    GLCanvasContent(ref, parent),
    m_debug(debug),
    m_renderer(nullptr),
    m_drawer(nullptr)
{
    setWindowTitle(tr("Display"));
    setWindowIcon(QIcon::fromTheme("input-gaming"));

    // add project name
    common::WorkspaceManager *workspaces = &common::Application::instance()->workspaces();
    if (workspaces->current()) {
        common::Project *project = workspaces->current()->project(ref);
        setWindowTitle(tr("Display %1").arg(project->name()));
    }
}

O3DCanvasContent::~O3DCanvasContent()
{
    if (m_drawer) {
        m_drawer->terminateDrawer();
    }
}

o3d::String O3DCanvasContent::elementName() const
{
    return String("o3s::main::o3dcanvascontent::") + m_ref.longId();
}

void O3DCanvasContent::setRenderer(QtRenderer *renderer)
{
    m_renderer = renderer;
}

const QtRenderer *O3DCanvasContent::renderer() const
{
    return m_renderer;
}

QtRenderer* O3DCanvasContent::renderer()
{
    return m_renderer;
}

void O3DCanvasContent::setDrawer(O3DDrawer *drawer)
{
    m_drawer = drawer;
}

void O3DCanvasContent::closeEvent(QCloseEvent *)
{
//    if (m_drawer) {
//        m_drawer->closed();
//    }
}

void O3DCanvasContent::initializeGL()
{
    if (!m_renderer) {
        return;
    }

    // first time creation
    if (!m_renderer->isValid()) {
        m_renderer->create(nullptr, m_debug);
    }

    if (m_drawer) {
        m_drawer->initializeDrawer();
    }

    // start the update loop
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(8);  // 8 ms update frequency

    // m_renderer->getContext()->setBackgroundColor(1.f, 1.f, 1.f, 1.f);
}

void O3DCanvasContent::paintGL()
{
    if (!m_renderer) {
        return;
    }

    m_renderer->clearAll();

    if (m_drawer) {
        m_drawer->paintDrawer();
    }
}

void O3DCanvasContent::resizeGL(int w, int h)
{
    if (!m_renderer) {
        return;
    }

    if (m_drawer) {
        m_drawer->resizeDrawer(w, h);
    }
}

void O3DCanvasContent::updateGL()
{
    if (m_drawer) {
        m_drawer->updateDrawer();
    }
}

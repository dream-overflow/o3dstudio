/**
 * @brief Common OpenGL 4.5 canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include "o3d/studio/common/ui/canvas/glcanvascontent.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;

GLCanvasContent::GLCanvasContent(const LightRef &ref, QWidget *parent) :
    QOpenGLWidget(parent),
    Content(),
    m_ref(ref)
{
    setWindowTitle(tr("Display"));
    setWindowIcon(QIcon::fromTheme("input-gaming"));

    // add project name
    common::WorkspaceManager *workspaces = &common::Application::instance()->workspaces();
    if (workspaces->current()) {
        common::Project *project = workspaces->current()->project(ref);
        setWindowTitle(tr("Display %1").arg(toQString(project->name())));
    }

    QSurfaceFormat format;
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);  // @todo why stay to 0 ?
    format.setVersion(4, 5);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    format.setProfile(QSurfaceFormat::CoreProfile);

    // must be called before the widget or its parent window gets shown
    setFormat(format);
}

GLCanvasContent::~GLCanvasContent()
{

}

o3d::String GLCanvasContent::elementName() const
{
    return String("o3s::main::glcanvascontent::") + m_ref.longId();
}

QWidget *GLCanvasContent::ui()
{
    return this;
}

QSize GLCanvasContent::sizeHint() const
{
    return QOpenGLWidget::sizeHint();  // return QSize(300, 200);
}

QSize GLCanvasContent::minimumSizeHint() const
{
    return QSize(100, 100);
}

void GLCanvasContent::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLCanvasContent::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);
}

void GLCanvasContent::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

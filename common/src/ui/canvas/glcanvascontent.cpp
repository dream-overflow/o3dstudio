/**
 * @brief Common OpenGL 4.5 canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include "common/ui/canvas/glcanvascontent.h"

using namespace o3d::studio::common;

GLCanvasContent::GLCanvasContent(QWidget *parent) :
    QOpenGLWidget(parent),
    Content()
{
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

QString GLCanvasContent::elementName() const
{
    // @todo need a suffix to have multiples
    return "o3s::main::glcanvascontent";
}

QWidget *GLCanvasContent::ui()
{
    return this;
}

QSize GLCanvasContent::sizeHint() const
{
    return QSize(300, 200);
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

}

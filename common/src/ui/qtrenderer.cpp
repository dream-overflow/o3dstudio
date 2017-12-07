/**
 * @file qtrenderer.cpp
 * @brief Specialisation of the O3D renderer supporting Qt OpenGLWidget.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @copyright Copyright (c) 2001-2017 Dream Overflow. All rights reserved.
 * @details
 */

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLContext>

#include "o3d/studio/common/ui/qtrenderer.h"

#include <o3d/core/gl.h>
#include <o3d/engine/glextdefines.h>
#include <o3d/engine/glextensionmanager.h>
#include <o3d/engine/context.h>

using namespace o3d::studio::common;


QtRenderer::QtRenderer(QOpenGLWidget *glWidget) :
    o3d::Renderer(),
    m_glWidget(glWidget)
{
    O3D_ASSERT(m_glWidget != nullptr);
}

QtRenderer::~QtRenderer()
{

}

void QtRenderer::create(o3d::AppWindow *appWindow, o3d::Bool debug, Renderer *sharing)
{
    Q_UNUSED(appWindow)
    Q_UNUSED(sharing)

    if (!m_glWidget) {
        return;
    }

    if (isValid()) {
        return;
    }

    try {
        GLExtensionManager::init();
    } catch(E_BaseException &e) {
        return;
    }

    m_state.enable(STATE_DEFINED);

    m_version = o3d::Renderer::Version(
                    m_glWidget->context()->format().version().first * 100 +
                    m_glWidget->context()->format().version().second * 10);

    m_appWindow = nullptr;

    m_bpp = m_glWidget->context()->format().redBufferSize() +
            m_glWidget->context()->format().greenBufferSize() +
            m_glWidget->context()->format().blueBufferSize() +
            m_glWidget->context()->format().alphaBufferSize();

    m_depth = m_glWidget->context()->format().depthBufferSize();
    m_stencil = m_glWidget->context()->format().stencilBufferSize();
    m_samples = m_glWidget->context()->format().samples();

    m_glContext = new o3d::Context(this);

    if (debug) {
        initDebug();
    }
}

void QtRenderer::destroy()
{
    if (m_state.getBit(STATE_DEFINED)) {
        if (m_refCount > 0) {
            O3D_ERROR(E_InvalidPrecondition("Unable to destroy a referenced renderer"));
        }

        if (m_shareCount > 0) {
            O3D_ERROR(E_InvalidPrecondition("All shared renderer must be destroyed before"));
        }

        // unshare
        if (m_sharing) {
            m_sharing->unrefShare();
            m_sharing = nullptr;
        }

        delete m_glContext;

        m_depth = m_bpp = m_stencil = m_samples = 0;

        m_state.zero();
        m_version = OGL_UNDEFINED;

        m_glErrno = GL_NO_ERROR;
    }
}

o3d::Bool QtRenderer::isCurrent() const
{
    if (m_glWidget) {
        return QOpenGLContext::currentContext() == m_glWidget->context();
    } else {
        return o3d::False;
    }
}

void QtRenderer::setCurrent()
{
    if (m_glWidget) {
        if (QOpenGLContext::currentContext() != m_glWidget->context()) {
            m_glWidget->makeCurrent();
        }
    }
}

o3d::Bool QtRenderer::setVSyncMode(VSyncMode mode)
{
    if (!m_glWidget) {
        return False;
    }

    if (!m_state.getBit(STATE_DEFINED)) {
        return False;
    }

    int value = 0;

    if (mode == VSYNC_NONE) {
        value = 0;
    } else if (mode == VSYNC_YES) {
        value = 1;
    } else if (mode == VSYNC_ADAPTIVE) {
        value = -1;
    }

    m_glWidget->format().setSwapInterval(value);

    if (mode == VSYNC_NONE) {
        m_state.setBit(STATE_VSYNC, False);
        m_state.setBit(STATE_ADAPTIVE_VSYNC, False);
    } else if (mode == VSYNC_YES) {
        m_state.setBit(STATE_VSYNC, True);
        m_state.setBit(STATE_ADAPTIVE_VSYNC, False);
    } else if (mode == VSYNC_ADAPTIVE) {
        m_state.setBit(STATE_VSYNC, True);
        m_state.setBit(STATE_ADAPTIVE_VSYNC, True);
    }

    return True;
}

QOpenGLWidget *QtRenderer::ui()
{
    return m_glWidget;
}

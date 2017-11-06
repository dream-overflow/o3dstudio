/**
 * @brief Common OpenGL 4.5 canvas content with O3D specialisation
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-01
 * @details
 */

#include <QtCore/QTimer>

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include <o3d/engine/context.h>

using namespace o3d::studio::common;

O3DCanvasContent::O3DCanvasContent(const QString &suffix, bool debug, QWidget *parent) :
    GLCanvasContent(suffix, parent),
    m_debug(debug),
    m_renderer(nullptr),
    m_drawer(nullptr)
{

}

O3DCanvasContent::~O3DCanvasContent()
{
    if (m_drawer) {
        m_drawer->terminateDrawer();
    }
}

QString O3DCanvasContent::elementName() const
{
    return "o3s::main::o3dcanvascontent::" + m_suffix;
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
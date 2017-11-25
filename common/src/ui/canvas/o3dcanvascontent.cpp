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

O3DCanvasContent::O3DCanvasContent(const LightRef &ref, Bool debug, QWidget *parent) :
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
        setWindowTitle(tr("Display %1").arg(toQString(project->name())));
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

    // paint every 16 ms
    static int i = 0;
    if (i == 0) {
        repaint();
        ++i;
    } else if (i == 1) {
        i = 0;
    }
}

#include "o3d/studio/common/ui/keyevent.h"
#include "o3d/studio/common/ui/mouseevent.h"
#include "o3d/studio/common/ui/wheelevent.h"
//#include "o3d/studio/common/ui/touchevent.h"
//#include "o3d/studio/common/ui/dragevent.h"
//#include "o3d/studio/common/ui/tabletevent.h"

void O3DCanvasContent::mousePressEvent(QMouseEvent *event)
{
    Point2f localPos(event->localPos().x(), event->localPos().y());
    Point2i globalPos(event->globalPos().x(), event->globalPos().y());

    Point2f screenPos(event->screenPos().x(), event->screenPos().y());
    Point2f windowPos(event->windowPos().x(), event->windowPos().y());

    MouseEvent evt(Event::MOUSE_BUTTON_PRESS,
                   event->timestamp(),
                   (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                   (unsigned int)event->button(),
                   (unsigned int)event->buttons(),
                   globalPos,
                   localPos,
                   screenPos,
                   windowPos);

    m_drawer->mousePressEvent(evt);
    QWidget::mousePressEvent(event);
}

void O3DCanvasContent::mouseReleaseEvent(QMouseEvent *event)
{
    Point2f localPos(event->localPos().x(), event->localPos().y());
    Point2i globalPos(event->globalPos().x(), event->globalPos().y());

    Point2f screenPos(event->screenPos().x(), event->screenPos().y());
    Point2f windowPos(event->windowPos().x(), event->windowPos().y());

    MouseEvent evt(Event::MOUSE_BUTTON_RELEASE,
                   event->timestamp(),
                   (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                   (unsigned int)event->button(),
                   (unsigned int)event->buttons(),
                   globalPos,
                   localPos,
                   screenPos,
                   windowPos);

    m_drawer->mouseReleaseEvent(evt);
    QWidget::mouseReleaseEvent(event);
}

void O3DCanvasContent::mouseDoubleClickEvent(QMouseEvent *event)
{
    Point2f localPos(event->localPos().x(), event->localPos().y());
    Point2i globalPos(event->globalPos().x(), event->globalPos().y());

    Point2f screenPos(event->screenPos().x(), event->screenPos().y());
    Point2f windowPos(event->windowPos().x(), event->windowPos().y());

    MouseEvent evt(Event::MOUSE_BUTTON_DBL_CLICK,
                   event->timestamp(),
                   (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                   (unsigned int)event->button(),
                   (unsigned int)event->buttons(),
                   globalPos,
                   localPos,
                   screenPos,
                   windowPos);

    m_drawer->mouseDoubleClickEvent(evt);
    QWidget::mouseDoubleClickEvent(event);
}

void O3DCanvasContent::mouseMoveEvent(QMouseEvent *event)
{
    Point2f localPos(event->localPos().x(), event->localPos().y());
    Point2i globalPos(event->globalPos().x(), event->globalPos().y());

    Point2f screenPos(event->screenPos().x(), event->screenPos().y());
    Point2f windowPos(event->windowPos().x(), event->windowPos().y());

    MouseEvent evt(Event::MOUSE_MOVE,
                   event->timestamp(),
                   (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                   (unsigned int)event->button(),
                   (unsigned int)event->buttons(),
                   globalPos,
                   localPos,
                   screenPos,
                   windowPos);

    m_drawer->mouseMoveEvent(evt);
    QWidget::mouseMoveEvent(event);
}

void O3DCanvasContent::wheelEvent(QWheelEvent *event)
{
    Point2i angleDelta(event->angleDelta().x(), event->angleDelta().y());

    Point2f globalPosF(event->globalPosF().x(), event->globalPosF().y());
    Point2f posF(event->posF().x(), event->posF().y());

    WheelEvent evt(Event::WHEEL_EVENT,
                   event->timestamp(),
                   (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                   (unsigned int)event->buttons(),
                   angleDelta,
                   globalPosF,
                   posF);

    m_drawer->wheelEvent(evt);
    QWidget::wheelEvent(event);
}

void O3DCanvasContent::keyPressEvent(QKeyEvent *event)
{
    KeyEvent evt(Event::KEY_PRESS_EVENT,
                 event->timestamp(),
                 event->key(),
                 (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                 // event->nativeScanCode(),
                 event->nativeVirtualKey(),
                 // event->nativeModifiers(),
                 fromQString(event->text()),
                 event->isAutoRepeat(),
                 event->count());

    m_drawer->keyPressEvent(evt);
    QWidget::keyPressEvent(event);
}

void O3DCanvasContent::keyReleaseEvent(QKeyEvent *event)
{
    KeyEvent evt(Event::KEY_RELEASE_EVENT,
                 event->timestamp(),
                 event->key(),
                 (InputEvent::KeyboardModifier)(unsigned int)event->modifiers(),
                 // event->nativeScanCode(),
                 event->nativeVirtualKey(),
                 // event->nativeModifiers(),
                 fromQString(event->text()),
                 event->isAutoRepeat(),
                 event->count());

    m_drawer->keyReleaseEvent(evt);
    QWidget::keyReleaseEvent(event);
}

void O3DCanvasContent::focusInEvent(QFocusEvent *event)
{
    Event evt(Event::FOCUS_IN);
    m_drawer->focusInEvent(evt);

    QWidget::focusInEvent(event);
}

void O3DCanvasContent::focusOutEvent(QFocusEvent *event)
{
    Event evt(Event::FOCUS_OUT);
    m_drawer->focusOutEvent(evt);

    QWidget::focusOutEvent(event);
}

void O3DCanvasContent::enterEvent(QEvent *event)
{
    Event evt(Event::ENTER);
    m_drawer->enterEvent(evt);

    QWidget::enterEvent(event);
}

void O3DCanvasContent::leaveEvent(QEvent *event)
{
    Event evt(Event::LEAVE);
    m_drawer->leaveEvent(evt);

    QWidget::leaveEvent(event);
}

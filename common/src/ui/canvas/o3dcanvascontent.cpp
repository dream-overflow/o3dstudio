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
    m_drawer(nullptr),
    m_timer(nullptr),
    m_queryRefresh(0),
    m_refreshBehavior(FULL_FREQUENCY_UPDATE),//SEMI_AUTO_UPDATE),
    m_repaint(False),
    m_private(nullptr)
{
    setWindowTitle(tr("Display"));
    setWindowIcon(QIcon(":/icons/videogame_asset_black.svg"));

    // add project name
    common::WorkspaceManager *workspaces = &common::Application::instance()->workspaces();
    if (workspaces->current()) {
        common::Project *project = workspaces->current()->project(ref);
        setWindowTitle(tr("Display %1").arg(toQString(project->name())));
    }

    m_private = new O3DCanvasContentPrivate(this);
}

O3DCanvasContent::~O3DCanvasContent()
{
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
    }

    if (m_drawer) {
        m_drawer->terminateDrawer();
    }

    deletePtr(m_private);
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

void O3DCanvasContent::setRefreshBehavior(O3DCanvasContent::RefreshBehavior refreshBehavior)
{
    if (refreshBehavior != m_refreshBehavior) {
        if (refreshBehavior == MANUAL_UPDATE) {
            if (m_timer) {
                m_timer->stop();
                deletePtr(m_timer);
            }
        } else if (refreshBehavior == SEMI_AUTO_UPDATE) {
            if (m_timer) {
                m_timer->stop();
                deletePtr(m_timer);
            }
        } else if (refreshBehavior == SLOW_FREQUENCY_UPDATE) {
            if (m_timer) {
                m_timer->setInterval(16);
            } else {
                m_timer = new QTimer(this);
                connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
                m_timer->start(16);  // 16 ms update frequency
            }
        } else if (refreshBehavior == FULL_FREQUENCY_UPDATE) {
            if (m_timer) {
                m_timer->setInterval(8);
            } else {
                m_timer = new QTimer(this);
                connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
                m_timer->start(8);  // 8 ms update frequency
            }
        }

        m_repaint = True;
        m_refreshBehavior = refreshBehavior;
    }
}

O3DCanvasContent::RefreshBehavior O3DCanvasContent::refreshBehavior() const
{
    return m_refreshBehavior;
}

void O3DCanvasContent::setDrawer(O3DDrawer *drawer)
{
    m_drawer = drawer;
}

void O3DCanvasContent::queryRefresh()
{
    m_private->onQueryRefresh();
}

o3d::Bool O3DCanvasContent::isDebug() const
{
    return m_debug;
}

o3d::Bool O3DCanvasContent::waitRefresh() const
{
    return m_queryRefresh > 0;
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
    if (m_refreshBehavior == SLOW_FREQUENCY_UPDATE) {
        if (!m_timer) {
            m_timer = new QTimer(this);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
            m_timer->start(16);  // 16 ms update frequency
        } else {
            m_timer->setInterval(16);
        }
    } else if (m_refreshBehavior == FULL_FREQUENCY_UPDATE) {
        if (!m_timer) {
            m_timer = new QTimer(this);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
            m_timer->start(8);  // 8 ms update frequency
        } else {
            m_timer->setInterval(8);
        }
    }

    m_repaint = True;
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

    // paint once on two
    if (m_repaint) {
        repaint();
        m_repaint = False;
    } else {
        m_repaint = True;
    }

    if (m_queryRefresh > 0) {
        --m_queryRefresh;
    }

    if (m_queryRefresh == 0) {
        // manual or semi auto, delete the temporary timer
        if (m_refreshBehavior == MANUAL_UPDATE || m_refreshBehavior == SEMI_AUTO_UPDATE) {
            m_timer->stop();
            deletePtr(m_timer);
        }
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
    if (m_drawer) {
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

        if (m_drawer->mousePressEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::mousePressEvent(event);
}

void O3DCanvasContent::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->mouseReleaseEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::mouseReleaseEvent(event);
}

void O3DCanvasContent::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->mouseDoubleClickEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::mouseDoubleClickEvent(event);
}

void O3DCanvasContent::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->mouseMoveEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::mouseMoveEvent(event);
}

void O3DCanvasContent::wheelEvent(QWheelEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->wheelEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::wheelEvent(event);
}

void O3DCanvasContent::keyPressEvent(QKeyEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->keyPressEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::keyPressEvent(event);
}

void O3DCanvasContent::keyReleaseEvent(QKeyEvent *event)
{
    if (m_drawer) {
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

        if (m_drawer->keyReleaseEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::keyReleaseEvent(event);
}

void O3DCanvasContent::focusInEvent(QFocusEvent *event)
{
    Event evt(Event::FOCUS_IN);
    if (m_drawer) {
        if (m_drawer->focusInEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::focusInEvent(event);
}

void O3DCanvasContent::focusOutEvent(QFocusEvent *event)
{
    Event evt(Event::FOCUS_OUT);
    if (m_drawer) {
        if (m_drawer->focusOutEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::focusOutEvent(event);
}

void O3DCanvasContent::enterEvent(QEvent *event)
{
    Event evt(Event::ENTER);
    if (m_drawer) {
        if (m_drawer->enterEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::enterEvent(event);
}

void O3DCanvasContent::leaveEvent(QEvent *event)
{
    Event evt(Event::LEAVE);
    if (m_drawer) {
        if (m_drawer->leaveEvent(evt)) {
            queryRefresh();
        }
    }

    QWidget::leaveEvent(event);
}

void O3DCanvasContent::queryRefreshPrivate()
{
    if (m_queryRefresh <= 0) {
        // only for manual and semi auto behaviors
        if (m_refreshBehavior == MANUAL_UPDATE || m_refreshBehavior == SEMI_AUTO_UPDATE) {
            // query 50 refresh
            m_queryRefresh = 50;

            // need a temporary timer at 8 ms
            if (!m_timer) {
                m_timer = new QTimer(this);
                connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
                m_timer->start(8);
            } else {
                m_timer->setInterval(8);
            }
        }
    }
}

O3DCanvasContentPrivate::O3DCanvasContentPrivate(O3DCanvasContent *content, BaseObject *parent) :
    BaseObject(parent),
    m_content(content)
{
    O3D_ASSERT(m_content != nullptr);

    // sync with main thread
    onQueryRefresh.connect(this, &O3DCanvasContentPrivate::queryRefresh, CONNECTION_ASYNCH);
}

void O3DCanvasContentPrivate::queryRefresh()
{
    m_content->queryRefreshPrivate();
}

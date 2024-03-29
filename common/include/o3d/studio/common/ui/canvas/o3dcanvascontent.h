/**
 * @brief Common OpenGL 4.5 canvas content with O3D specialisation
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_O3DCANVASCONTENT_H
#define _O3DS_COMMON_O3DCANVASCONTENT_H

#include "glcanvascontent.h"
#include "o3ddrawer.h"
#include "../../objectref.h"

#include <o3d/core/baseobject.h>

namespace o3d {

class Renderer;

namespace studio {
namespace common {

class O3S_API O3DCanvasContentPrivate : public BaseObject
{
public:

    O3DCanvasContentPrivate(class O3DCanvasContent *content, BaseObject *parent = nullptr);

public /* slots */:

    void queryRefresh();

public /*signals*/:

    Signal<> onQueryRefresh{this};

private:

    class O3DCanvasContent *m_content;
};

class O3S_API O3DCanvasContent : public GLCanvasContent
{
    Q_OBJECT

    friend class O3DCanvasContentPrivate;

public:

    enum RefreshBehavior
    {
        MANUAL_UPDATE = 0,          //!< Update and paint must be called manually, only Qt event do paint
        SEMI_AUTO_UPDATE = 1,       //!< Edition mode, not waste CPU, update and repaint less possible
        SLOW_FREQUENCY_UPDATE = 2,  //!< Half game mode, update and repaint at slow frequency (uses of timer)
        FULL_FREQUENCY_UPDATE = 3   //!< Full game mode, update and repaint at the best frequency (uses of timer)
    };

    explicit O3DCanvasContent(const LightRef &ref, Bool debug = false, QWidget *parent = nullptr);
    virtual ~O3DCanvasContent();

    virtual String elementName() const override;

    void setRenderer(Renderer *renderer);

    const Renderer *renderer() const;
    Renderer *renderer();

    /**
     * @brief setBehavior
     * @param behavior
     */
    void setRefreshBehavior(RefreshBehavior refreshBehavior);
    RefreshBehavior refreshBehavior() const;

    void setDrawer(O3DDrawer *drawer);
    void queryRefresh();

    Bool isDebug() const;
    Bool waitRefresh() const;

public slots:

    void updateGL();

protected:

    virtual void closeEvent(QCloseEvent*) override;

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:

    Bool m_debug;                       //!< Renderer in debug mode
    Renderer *m_renderer;               //!< Attached renderer

    O3DDrawer *m_drawer;                //!< Attached drawer
    QTimer *m_timer;                    //!< Refresh timer

    Int32 m_queryRefresh;               //!< How many refresh needed
    RefreshBehavior m_refreshBehavior;  //!< Current refresh behavior

    Bool m_repaint;

    O3DCanvasContentPrivate *m_private;

    void queryRefreshPrivate();
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DCANVASCONTENT_H

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
#include "../qtrenderer.h"
#include "o3ddrawer.h"
#include "../../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API O3DCanvasContent : public GLCanvasContent
{
    Q_OBJECT

public:

    explicit O3DCanvasContent(const LightRef &ref, Bool debug = false, QWidget *parent = nullptr);
    virtual ~O3DCanvasContent();

    virtual String elementName() const override;

    void setRenderer(QtRenderer *renderer);

    const QtRenderer *renderer() const;
    QtRenderer *renderer();

    void setDrawer(O3DDrawer *drawer);

public slots:

    void updateGL();

protected:

    virtual void closeEvent(QCloseEvent*);

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

private:

    Bool m_debug;
    QtRenderer *m_renderer;

    O3DDrawer *m_drawer;
    QTimer *m_timer;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DCANVASCONTENT_H

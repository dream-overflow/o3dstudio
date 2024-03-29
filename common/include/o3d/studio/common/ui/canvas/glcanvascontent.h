/**
 * @brief Common OpenGL 4.5 canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_GLCANVASCONTENT_H
#define _O3DS_COMMON_GLCANVASCONTENT_H

#include <QtOpenGL/QtOpenGL>

#include "../content.h"
#include "../../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API GLCanvasContent : public QOpenGLWidget, public Content
{
public:

    explicit GLCanvasContent(const LightRef &ref, QWidget *parent = nullptr);
    virtual ~GLCanvasContent();

    virtual String elementName() const override;
    virtual QWidget* ui() override;

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

public slots:

protected:

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

protected:

    LightRef m_ref;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_GLCANVASCONTENT_H

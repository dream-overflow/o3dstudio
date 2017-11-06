/**
 * @brief Common OpenGL 4.5 canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_GLCANVASCONTENT_H
#define _O3DS_COMMON_GLCANVASCONTENT_H

#include <QtWidgets/QWidget>
#include <QtOpenGL>

#include "../content.h"

namespace o3d {
namespace studio {
namespace common {

class GLCanvasContent : public QOpenGLWidget, public Content
{
public:

    explicit GLCanvasContent(const QString &suffix, QWidget *parent = nullptr);
    virtual ~GLCanvasContent();

    virtual QString elementName() const override;
    virtual QWidget* ui() override;

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

public slots:

protected:

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

protected:

    QString m_suffix;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_GLCANVASCONTENT_H

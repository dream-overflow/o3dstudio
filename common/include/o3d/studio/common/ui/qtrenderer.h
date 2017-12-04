/**
 * @file qtrenderer.h
 * @brief Specialisation of the O3D renderer supporting Qt OpenGLWidget.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @copyright Copyright (c) 2001-2017 Dream Overflow. All rights reserved.
 * @details
 */

#ifndef _O3DS_COMMON_QTRENDERER_H
#define _O3DS_COMMON_QTRENDERER_H

#include <o3d/engine/renderer.h>

#include "../global.h"

class QOpenGLWidget;

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Renderer class for Qt
 * @date 2017-11-01
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 */
class O3S_API QtRenderer : public o3d::Renderer
{
public:

    //! Default constructor.
    QtRenderer(QOpenGLWidget *glWidget);

    //! Virtual destructor.
    virtual ~QtRenderer();

    virtual void create(o3d::AppWindow *appWindow, o3d::Bool debug = False, o3d::Renderer *sharing = nullptr) override;
    virtual void destroy() override;

    virtual o3d::Bool isCurrent() const override;
    virtual void setCurrent() override;

    virtual void setVerticalRefresh(o3d::Bool use) override;
    virtual o3d::Bool isVerticalRefresh() const override;

    QOpenGLWidget *ui();

private:

    QOpenGLWidget *m_glWidget;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_QTRENDERER_H

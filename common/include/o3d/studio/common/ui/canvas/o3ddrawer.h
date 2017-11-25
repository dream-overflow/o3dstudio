/**
 * @brief Common interface for object that have capacity to draw inside an O3DCanvasContent object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_O3DRAWER_H
#define _O3DS_COMMON_O3DRAWER_H

#include "../../global.h"
#include "../keyevent.h"
#include "../mouseevent.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API O3DDrawer
{
public:

    virtual void initializeDrawer() = 0;
    virtual void paintDrawer() = 0;
    virtual void updateDrawer() = 0;
    virtual void resizeDrawer(int w, int h) = 0;
    virtual void terminateDrawer() = 0;

    virtual void mousePressEvent(const MouseEvent &event) = 0;
    virtual void mouseReleaseEvent(const MouseEvent &event) = 0;
    virtual void mouseDoubleClickEvent(const MouseEvent &event) = 0;
    virtual void mouseMoveEvent(const MouseEvent &event) = 0;
    virtual void wheelEvent(const MouseEvent &event) = 0;
    virtual void keyPressEvent(const KeyEvent &event) = 0;
    virtual void keyReleaseEvent(const KeyEvent &event) = 0;
    virtual void focusInEvent(const Event &event) = 0;
    virtual void focusOutEvent(const Event &event) = 0;
    virtual void enterEvent(const Event &event) = 0;
    virtual void leaveEvent(const Event &event) = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DRAWER_H

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
#include "../wheelevent.h"

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

    virtual Bool mousePressEvent(const MouseEvent &event) = 0;
    virtual Bool mouseReleaseEvent(const MouseEvent &event) = 0;
    virtual Bool mouseDoubleClickEvent(const MouseEvent &event) = 0;
    virtual Bool mouseMoveEvent(const MouseEvent &event) = 0;
    virtual Bool wheelEvent(const WheelEvent &event) = 0;
    virtual Bool keyPressEvent(const KeyEvent &event) = 0;
    virtual Bool keyReleaseEvent(const KeyEvent &event) = 0;
    virtual Bool focusInEvent(const Event &event) = 0;
    virtual Bool focusOutEvent(const Event &event) = 0;
    virtual Bool enterEvent(const Event &event) = 0;
    virtual Bool leaveEvent(const Event &event) = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DRAWER_H

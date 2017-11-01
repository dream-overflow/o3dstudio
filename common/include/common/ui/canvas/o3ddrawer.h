/**
 * @brief Common interface for object that have capacity to draw inside an O3DCanvasContent object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_O3DRAWER_H
#define _O3DS_COMMON_O3DRAWER_H

namespace o3d {
namespace studio {
namespace common {

class O3DDrawer
{
public:

    virtual void initializeDrawer() = 0;
    virtual void paintDrawer() = 0;
    virtual void resizeDrawer(int w, int h) = 0;
    virtual void terminateDrawer() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DRAWER_H

/**
 * @brief Common UI scene grid controller
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#ifndef _O3DS_COMMON_GRID_H
#define _O3DS_COMMON_GRID_H

#include "sceneuielement.h"

#include <o3d/core/vector2.h>
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Grid helper attached to each master scene.
 * Uses of directRendering method for now.
 */
class O3S_API Grid : public SceneUIElement
{
public:

    /**
     * @brief Grid
     * @param pos 3d position of the grid
     * @param halfSize Half size on X and Y (must be a multiple of step)
     * @param step Increment of the grid lines
     */
    Grid(const Point3f &pos, const Point2i &halfSize, const Point2i &step);
    virtual ~Grid();

    virtual void createToScene(MasterScene *masterScene);
    virtual void removeFromScene(MasterScene *masterScene);
    virtual void syncWithScene(MasterScene *masterScene);
    virtual void directRendering(MasterScene *MasterScene);

protected:

    Point3f m_pos;
    Point2i m_halfSize;
    Point2i m_step;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_GRID_H

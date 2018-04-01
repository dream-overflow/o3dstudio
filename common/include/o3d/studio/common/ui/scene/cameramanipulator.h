/**
 * @brief Common UI scene camera manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @details
 */

#ifndef _O3DS_CAMERAMANIPULATOR_H
#define _O3DS_CAMERAMANIPULATOR_H

#include "sceneuielement.h"

#include <o3d/core/vector2.h>
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The working camera helper attached to each master scene.
 */
class O3S_API CameraManipulator : public SceneUIElement
{
public:

    /**
     * @brief CameraManipulator
     * @param parent Parent object
     * @param pos Position in percent of the display
     * @param scale Scale of the object (default 1.0)
     */
    CameraManipulator(BaseObject *parent, const Point2f &pos, const Float scale = 1.f);
    virtual ~CameraManipulator();

    virtual void setup(MasterScene *masterScene) override;
    virtual void release(MasterScene *masterScene) override;

    virtual void hover(UInt32 id, const Point3f &pos) override;
    virtual void leave() override;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene) override;

protected:

    Point2f m_pos;
    Float m_scale;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_CAMERAMANIPULATOR_H

/**
 * @brief Common UI scene hub manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-24
 * @details
 */

#ifndef _O3DS_HUBMANIPULATOR_H
#define _O3DS_HUBMANIPULATOR_H

#include "sceneuielement.h"

#include <o3d/core/vector2.h>
#include <o3d/core/vector3.h>
#include <o3d/core/matrix4.h>
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace common {

class Hub;

/**
 * @brief The working hub helper attached to current master scene and active visible hub.
 */
class O3S_API HubManipulator : public SceneUIElement
{
public:

    /**
     * @brief HubManipulator
     * @param parent Parent object
     * @param target Targeted hub object.
     * @param hitPos Position where the hit (picking) occured
     */
    HubManipulator(BaseObject *parent, Hub* target, const Matrix4 &transform);

    /**
     * @brief HubManipulator
     * @param parent Parent object
     * @param targets Targeted list of hubs objects.
     * @param hitPos Position where the hit (picking) occured
     */
    HubManipulator(BaseObject *parent, std::list<Hub*> targets, const Matrix4 &transform);

    virtual ~HubManipulator();

    virtual void setup(MasterScene *masterScene) override;
    virtual void release(MasterScene *masterScene) override;

    virtual void hover(UInt32 id, const Point3f &pos) override;
    virtual void leave() override;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene) override;

protected:

    enum Axe
    {
        AXE_NONE = -1,
        AXE_X = 0,
        AXE_Y = 1,
        AXE_Z = 2,
        AXE_MANY = 4
    };

    std::list<Hub*> m_targets;
    Matrix4 m_transform;

    Float m_scale;

    UInt32 m_pickingMask;

    Int32 m_axe;    //!< Current axe or -1 if none
    Float m_delta;  //!< Current delta of the transform

    Color axeColor(Axe axe);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_HUBMANIPULATOR_H

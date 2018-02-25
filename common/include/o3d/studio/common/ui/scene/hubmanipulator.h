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
    HubManipulator(BaseObject *parent, Hub* target, const Point3f &hitPos);

    /**
     * @brief HubManipulator
     * @param parent Parent object
     * @param targets Targeted list of hubs objects.
     * @param hitPos Position where the hit (picking) occured
     */
    HubManipulator(BaseObject *parent, std::list<Hub*> targets, const Point3f &hitPos);

    virtual ~HubManipulator();

    virtual void createToScene(MasterScene *masterScene);
    virtual void removeFromScene(MasterScene *masterScene);
    virtual void syncWithScene(MasterScene *masterScene);
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene);

protected:

    std::list<Hub*> m_targets;
    Point3f m_hitPos;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_HUBMANIPULATOR_H

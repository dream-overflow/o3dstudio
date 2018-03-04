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
#include <o3d/core/quaternion.h>
#include <o3d/image/color.h>

namespace o3d {

class Transform;

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
    HubManipulator(BaseObject *parent, Hub* target);

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

    virtual void beginTransform(MasterScene *masterScene);
    virtual void transform(const Vector3f &v, MasterScene *masterScene);
    virtual void endTransform();
    virtual Bool isTransform() const;

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

    enum TransformMode
    {
        STATIC = 0,
        TRANSLATE,
        ROTATE,
        SCALE,
        SKEW
    };

    enum TransformAxis
    {
        TR_LOCAL = 0,   //!< Local axis of the selected object.
        TR_VIEW,        //!< Related to the current view axis.
        TR_GLOBAL,      //!< Absolute (origin).
        TR_USER,        //!< An arbitrary axis defined by user.
        TR_MEDIAN       //!< Computed median of the selection.
    };

    std::list<Hub*> m_targets;
    std::list<Vector3> m_orgV;

    Transform *m_transform;  //!< Current transformation of the helpers.
    Vector3 m_position;      //!< Original position of the helper.
    Vector3 m_rotation;      //!< Original rotation of the helper.
    Vector3 m_scale;         //!< Original scale of the helper.

    Float m_displayScale;

    UInt32 m_pickingMask;

    Int32 m_axe;                    //!< Current axe or -1 if none.
    Vector3f m_delta;               //!< Current delta of the transform.
    TransformMode m_transformMode;  //!< Current transform (when beginTransform...).
    TransformAxis m_transformAxis;  //!< Current transform axis.

    void updateTransform(MasterScene *masterScene);
    Color axeColor(Axe axe);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_HUBMANIPULATOR_H

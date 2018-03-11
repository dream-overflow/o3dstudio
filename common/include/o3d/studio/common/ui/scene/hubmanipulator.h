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
class SpacialNodeHub;
class KeyEvent;

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
    HubManipulator(BaseObject *parent, const std::list<Hub *> targets);

    virtual ~HubManipulator();

    virtual void setup(MasterScene *masterScene) override;
    virtual void release(MasterScene *masterScene) override;

    virtual void hover(UInt32 id, const Point3f &pos) override;
    virtual void leave() override;

    //! May be removed, need a dedicated toolbar, but need shortcut too
    virtual void keyDownEvent(const KeyEvent &event, MasterScene *masterScene);

    virtual void beginTransform(MasterScene *masterScene, const Vector3f &pos);
    virtual void transform(const Vector3f &v, MasterScene *masterScene);
    virtual void endTransform();
    virtual void cancelTransform(MasterScene *masterScene);
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

    /**
     * @brief Pivot point is...
     */
    enum PivotPoint
    {
        PIVOT_ACTIVE_ELT = 0,   //!< on the active element of the selection.
        PIVOT_INDIVIDUAL,       //!< relative for each element.
        PIVOT_MEDIAN,           //!< relative to the computed median point.
        PIVOT_USER             //!< relative to used defined point.
    };

    /**
     * @brief Transform orientation is...
     */
    enum TransformOrientation
    {
        TR_LOCAL = 0,   //!< Local axis of the selected object.
        TR_GLOBAL,      //!< Absolute (origin).
        TR_VIEW         //!< Related to the current view axis.
    };

    std::list<Hub*> m_targets;
    std::list<Vector3> m_orgV;
    std::list<Quaternion> m_orgQ;

    Transform *m_transform;  //!< Current helper transformation to apply.
    Vector3 m_offset;        //!< Helper offset for some transform mode.

    Vector3 m_orgPos;      //!< Original position of the helper.
    Quaternion m_orgRot;   //!< Original rotation of the helper.
    Vector3 m_orgScale;    //!< Original scale of the helper.

    Float m_displayScale;

    UInt32 m_pickingMask;

    Axe m_axe;                      //!< Current axe or -1 if none.
    Vector3f m_relativeV;           //!< Current delta of the transform.

    Vector3f m_initial;             //!< Initial input position.
    Vector3f m_previous;              //!< Current sum of the delta (not transformed).

    TransformMode m_transformMode;  //!< Current transform (when beginTransform...).
    PivotPoint m_pivotPoint;        //!< Current defined pivot point.
    TransformOrientation m_transformOrientation;  //!< Current transform orientation mode.

    SpacialNodeHub *m_activeElt;

    void updateTransform(MasterScene *masterScene);
    void refresh(MasterScene *masterScene);

    Vector3f computeLinearVelocity(MasterScene *masterScene, const Vector3f &delta, Axe axe);
    Vector3f computeCircularVelocity(MasterScene *masterScene, const Vector3f &delta, Axe axe);

    Color axeColor(Axe axe);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_HUBMANIPULATOR_H

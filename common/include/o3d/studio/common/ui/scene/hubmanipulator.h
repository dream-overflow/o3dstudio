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

class StructuralHub;
class SpacialNodeHub;
class KeyEvent;

/**
 * @brief The working hub helper attached to current master scene and active visible hub.
 */
class O3S_API HubManipulator : public SceneUIElement
{
public:

    /**
     * @brief Pivot point is defined :
     */
    enum PivotPoint
    {
        PIVOT_ACTIVE_ELT = 0,   //!< on the active element of the selection.
        PIVOT_INDIVIDUAL,       //!< relative for each element.
        PIVOT_MEDIAN,           //!< relative to the computed median point.
        PIVOT_USER              //!< relative to used defined point.
    };

    /**
     * @brief Transform orientation is base on :
     */
    enum TransformOrientation
    {
        TR_LOCAL = 0,   //!< Local axis of the selected object.
        TR_GLOBAL,      //!< Absolute (origin).
        TR_VIEW         //!< Related to the current view axis.
    };

    /**
     * @brief Used transformation mode.
     */
    enum TransformMode
    {
        TRANSLATE,
        ROTATE,
        SCALE,
        SKEW
    };

    /**
     * @brief HubManipulator
     * @param parent Parent object
     */
    HubManipulator(BaseObject *parent);

    virtual ~HubManipulator();

    virtual void setup(MasterScene *masterScene) override;
    virtual void release(MasterScene *masterScene) override;

    virtual void hover(UInt32 id, const Point3f &pos) override;
    virtual void leave() override;

    virtual void setSelection(MasterScene *masterScene, const std::list<StructuralHub *> targets);
    virtual void setActiveHub(MasterScene *masterScene, StructuralHub *hub);

    virtual Bool hasSelection() const;
    virtual StructuralHub* activeHub();

    //! May be removed, need a dedicated toolbar, but need shortcut too
    virtual void keyDownEvent(const KeyEvent &event, MasterScene *masterScene);

    virtual void beginTransform(MasterScene *masterScene, const Vector3f &pos);
    virtual void transform(const Vector3f &v, MasterScene *masterScene);
    virtual void endTransform(MasterScene *masterScene);
    virtual void cancelTransform(MasterScene *masterScene);
    virtual Bool isTransform() const;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene) override;

    /**
     * @brief setPivotPoint
     * @param mode
     * @note Not possible during a transformation.
     */
    void setPivotPoint(PivotPoint mode);

    /**
     * @brief setTransformOrientation
     * @param mode
     * @note Not possible during a transformation.
     */
    void setTransformOrientation(TransformOrientation mode);

    /**
     * @brief setTransforMode
     * @param mode
     * @note Not possible during a transformation.
     */
    void setTransforMode(TransformMode mode);

protected:

    enum Axe
    {
        AXE_NONE = -1,
        AXE_X = 0,
        AXE_Y = 1,
        AXE_Z = 2,
        AXE_MANY = 4
    };

    std::list<StructuralHub*> m_targets;
    std::list<Vector3> m_orgV;
    std::list<Quaternion> m_orgQ;

    Transform *m_transform;  //!< Current helper transformation to apply.
    Vector3 m_offset;        //!< Helper offset for some transform mode.

    Vector3 m_orgPos;      //!< Original position of the helper.
    Quaternion m_orgRot;   //!< Original rotation of the helper.
    Vector3 m_orgScale;    //!< Original scale of the helper.

    Float m_displayScale;

    Axe m_hoverAxe;                 //!< Hover axe or -1 if none.
    Axe m_activeAxe;                //!< Active (transform) axe or -1 if none.
    Vector3f m_relativeV;           //!< Current delta of the transform.

    Vector3f m_initial;             //!< Initial input position.
    Vector3f m_previous;            //!< Current sum of the delta (not transformed).

    Bool m_transforming;
    TransformMode m_transformMode;  //!< Current transform (when beginTransform...).
    PivotPoint m_pivotPoint;        //!< Current defined pivot point.
    TransformOrientation m_transformOrientation;  //!< Current transform orientation mode.

    Vector3f m_actionPos;           //!< Current 2D position
    Float m_actionRadius;           //!< Radius of action
    Float m_actionPhi;              //!< Angle of the action from initial

    StructuralHub *m_activeElt;

    void updateTransform(MasterScene *masterScene, Bool keepOrg);
    void refresh(MasterScene *masterScene);

    Vector3f computeLinearVelocity(MasterScene *masterScene, const Vector3f &delta, Axe axe);
    Vector3f computeCircularVelocity(MasterScene *masterScene, const Vector3f &delta, Axe axe);

    Color axeColor(Axe axe);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_HUBMANIPULATOR_H

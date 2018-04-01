/**
 * @brief Common scene UI element base class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#ifndef _O3DS_COMMON_SCENEUI_H
#define _O3DS_COMMON_SCENEUI_H

#include "../../global.h"

#include <o3d/core/smartobject.h>
#include <o3d/core/vector3.h>

namespace o3d {

class Scene;
class DrawInfo;

namespace studio {
namespace common {

class MasterScene;

/**
 * @brief Common scene UI element base class
 * @todo input event and move some from master scene to elements
 */
class O3S_API SceneUIElement : public BaseObject
{
public:

    enum UIType
    {
        SCENE_UI_2D,        //!< 2d displayed element into the scene
        SCENE_UI_3D,        //!< 3d displayed element into the scene
        SCENE_UI_3D_LABEL   //!< 3d as label (constant size) element into the scene
    };

    enum DrawStep
    {
        PRE_DRAW,       //!< draw before the scene
        MANAGED_DRAW,   //!< managed by a specific controller
        POST_DRAW       //!< draw after the scene
    };

    enum Mode
    {
        MODE_SELECTION,      //!< scene object selection mode
        MODE_EDITION         //!< scene object edition mode
    };

    SceneUIElement(BaseObject *parent, UIType uiType, DrawStep drawStep, Bool directDraw);

    virtual ~SceneUIElement();

    //
    // global
    //

    /**
     * @brief Setup for the given master scene.
     * @param masterScene Valid master scene instance.
     */
    virtual void setup(MasterScene *masterScene);

    /**
     * @brief Release on remove from the master scene.
     * @param masterScene Valid master scene instance.
     */
    virtual void release(MasterScene *masterScene);

    /**
     * @brief Set or reset focus.
     */
    void setFocus(Bool focus = True);

    /**
     * @brief Has current focus.
     */
    Bool hasFocus() const;

    /**
     * @brief A part the element is hover.
     * @param id Element or sub-element id.
     * @param pos Position where the hover occurs.
     */
    virtual void hover(UInt32 id, const Point3f &pos);

    /**
     * @brief Leave the element if previously hover.
     */
    virtual void leave();

    //
    // engine proxy
    //

    /**
     * @return True if uses of direct rendering, else uses scene rendering.
     */
    Bool isDirectDraw() const;

    /**
     * @return Draw step.
     */
    DrawStep drawStep() const;

    /**
     * @brief Create the related entities into the engine scene.
     */
    virtual void createToScene(MasterScene *masterScene) = 0;

    /**
     * @brief Destroy the related entities fomr the engine scene.
     */
    virtual void removeFromScene(MasterScene *masterScene) = 0;

    /**
     * @brief Bi-directionnal synchronization with the engine scene.
     */
    virtual void syncWithScene(MasterScene *masterScene) = 0;

    /**
     * @brief Or uses of a direct rendering. Called during scene drawer paint.
     */
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene) = 0;

protected:

    UIType m_uiType;       //!< Type of scene UI element
    DrawStep m_drawStep;   //!< Draw step
    Bool m_directDraw;     //!< True mean call directRendering, else uses the 3 others methods
    Bool m_focus;          //!< True if the ui element has current focus
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SCENEUI_H

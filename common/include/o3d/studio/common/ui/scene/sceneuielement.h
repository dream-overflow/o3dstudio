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
#include <o3d/engine/scene/sceneobject.h>
#include <o3d/engine/shader/shadable.h>

namespace o3d {
namespace studio {
namespace common {

class MasterScene;

/**
 * @brief Common scene UI element base class
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

    SceneUIElement(BaseObject *parent, UIType uiType, DrawStep drawStep, Bool directDraw);

    virtual ~SceneUIElement();

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
    virtual void directRendering(Scene *scene) = 0;

protected:

    UIType m_uiType;       //!< Type of scene UI element
    DrawStep m_drawStep;   //!< Draw step
    Bool m_directDraw;     //!< True mean call directRendering, else uses the 3 others methods
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SCENEUI_H

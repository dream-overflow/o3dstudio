/**
 * @brief Default master scene drawer.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @copyright Copyright (c) 2001-2017 Dream Overflow. All rights reserved.
 * @details
 */

#ifndef _O3DS_COMMON_MASTERSCENEDRAWER_H
#define _O3DS_COMMON_MASTERSCENEDRAWER_H

#include <o3d/core/classinfo.h>
#include <o3d/engine/shadow/shadowvolumeforward.h>

#include "../ui/scene/sceneuielement.h"

#include <list>

namespace o3d {
namespace studio {
namespace common {

class MasterScene;

/**
 * @brief Shadow volume rendering using forward light.
 */
class O3S_API MasterSceneDrawer : public ShadowVolumeForward
{
public:

    O3D_DECLARE_DYNAMIC_CLASS(MasterSceneDrawer)

    //! Constructor. Take a parent objets.
    MasterSceneDrawer(BaseObject *parent);

    //! Draw process.
    virtual void draw();

    //! Process a simpler draw for the picking pass.
    virtual void drawPicking();

    //! Add scene UI element.
    void addSceneUIElement(SceneUIElement *elt);

    //! Remove scene UI element (not delete).
    void removeSceneUIElement(SceneUIElement *elt);

protected:

    std::list<SceneUIElement*> m_preDraw;
    std::list<SceneUIElement*> m_postDraw;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENEDRAWER_H

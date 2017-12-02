/**
 * @brief Common UI scene info HUD view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-01
 * @details
 */

#ifndef _O3DS_COMMON_INFOHUD_H
#define _O3DS_COMMON_INFOHUD_H

#include "sceneuielement.h"

#include <o3d/core/smartobject.h>
#include <o3d/core/vector2.h>
#include <o3d/core/vector3.h>
#include <o3d/image/color.h>

namespace o3d {

class Text2D;

namespace studio {
namespace common {

/**
 * @brief The info HUD attached to each master scene.
 * @details Display common current scene details about FPS, tris, faces...
 * Uses of directRendering method for now.
 */
class O3S_API InfoHUD : public SceneUIElement
{
public:

    InfoHUD(BaseObject *parent, const Point2i &pos, Text2D *font);
    virtual ~InfoHUD();

    virtual void createToScene(MasterScene *masterScene);
    virtual void removeFromScene(MasterScene *masterScene);
    virtual void syncWithScene(MasterScene *masterScene);
    virtual void directRendering(Scene *scene);

protected:

    Point2i m_pos;

    SmartObject<Text2D> m_font;

    Color m_textColor;
    Color m_bgColor;

    Int32 m_rowHeight;

    // @todo a list of text properties to display
    // @todo spacing between lines
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_INFOHUD_H

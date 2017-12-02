/**
 * @brief Common UI scene info HUD view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-01
 * @details
 */

#include "o3d/studio/common/ui/scene/infohud.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/object/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/text2d.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


InfoHUD::InfoHUD(BaseObject *parent, const Point2f &pos, Text2D *font) :
    SceneUIElement(parent, SCENE_UI_2D, POST_DRAW, True),
    m_pos(pos),
    m_font(this, font),
    m_textColor(0.f, 1.f, 0.f, 1.f),
    m_bgColor(0.f, 0.f, 0.f, 0.f),
    m_rowHeight(font->getTextHeight() + font->getInterline())
{

}

InfoHUD::~InfoHUD()
{

}

void InfoHUD::createToScene(MasterScene *Scene)
{

}

void InfoHUD::removeFromScene(MasterScene *)
{

}

void InfoHUD::syncWithScene(MasterScene *)
{

}

void InfoHUD::directRendering(Scene *scene)
{
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    // setup modelview
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix());



    // @todo one line per property
}

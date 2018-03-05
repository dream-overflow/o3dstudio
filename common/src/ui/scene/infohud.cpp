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
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/text2d.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/utils/framemanager.h>

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

void InfoHUD::directRendering(DrawInfo &drawInfo, MasterScene *masterScene)
{
    Scene *scene = masterScene->scene();

    const Box2i &vp = scene->getContext()->getViewPort();

    // setup modelview
    scene->getContext()->modelView().set(scene->getActiveCamera()->getModelviewMatrix());

    scene->getContext()->enableDepthTest();
    scene->getContext()->enableDepthWrite();
    scene->getContext()->setDefaultDepthTest();
    scene->getContext()->setDefaultDepthFunc();

    // setup modelview
    Matrix4 mv;
    mv.setTranslation(5, 5+m_font.get()->getTextHeight(), 0.f);
    scene->getContext()->modelView().set(mv);

    // and project to ortho
    Matrix4 pj;
    Matrix4 oldPj = scene->getContext()->projection().get();
    pj.buildOrtho(vp.x(), vp.x2(), vp.y2(), vp.y(), -1.f, 1.f);
    scene->getContext()->projection().set(pj);

//    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_HINT_NICEST);
    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);

    scene->getContext()->setCullingMode(CULLING_BACK_FACE);
    scene->getContext()->disableDoubleSide();
    scene->getContext()->disableDepthTest();
    scene->getContext()->disableDepthWrite();

    m_font->setColor(Color(0.1f, 0.9f, 0.1f));

    // one line per property
    Int32 i = 0;
    m_font->writeAtRow(i++, 0, String("Vertices : {0}").arg(masterScene->numVertices(1)));
    m_font->writeAtRow(i++, 0, String("Triangles : {0}").arg(masterScene->numTriangles(1)));
    m_font->writeAtRow(i++, 0, String("Lines: {0}").arg(masterScene->numLines(1)));
    m_font->writeAtRow(i++, 0, String("Points : {0}").arg(masterScene->numPoints(1)));

    // restore
    scene->getContext()->projection().set(oldPj);
    scene->getContext()->setAntiAliasing(aa);
}

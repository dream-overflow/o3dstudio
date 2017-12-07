/**
 * @brief Common UI scene grid view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#include "o3d/studio/common/ui/scene/grid.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/object/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


Grid::Grid(BaseObject *parent, const Point3f &pos, const Point2i &halfSize, const Point2i &step) :
    SceneUIElement(parent, SCENE_UI_3D, PRE_DRAW, True),
    m_pos(pos),
    m_halfSize(halfSize),
    m_step(step)
{

}

Grid::~Grid()
{

}

void Grid::createToScene(MasterScene *)
{

}

void Grid::removeFromScene(MasterScene *)
{

}

void Grid::syncWithScene(MasterScene *)
{

}

void Grid::directRendering(DrawInfo &drawInfo, MasterScene *masterScene)
{
    // only on ambient pass
    if (drawInfo.pass != DrawInfo::AMBIENT_PASS) {
        return;
    }

    Scene *scene = masterScene->scene();
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    // setup modelview
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix());

    scene->getContext()->enableDepthTest();
    scene->getContext()->enableDepthWrite();

    primitive->modelView().push();
    primitive->modelView().translate(m_pos);

    // Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_HINT_NICEST);
    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);

    scene->getContext()->setLineWidth(2.0f);

    // intensity
    primitive->setColor(1.f, 1.f, 1.f);

    primitive->beginDraw(P_LINES);

    Color light(0.85f, 0.85f, 0.85f);
    Color dark(0.6f, 0.6f, 0.6f);

    // on Y axis
    for (Int32 y = -m_halfSize.y() ; y < 0 ; y += m_step.y()) {
        primitive->addVertex(Vector3(-m_halfSize.x(), 0, y), light);
        primitive->addVertex(Vector3(m_halfSize.x(), 0, y), light);
    }

    // origin line
    primitive->addVertex(Vector3(-m_halfSize.x(), 0, 0), dark);
    primitive->addVertex(Vector3(m_halfSize.x(), 0, 0), dark);

    for (Int32 y = m_step.y() ; y <= m_halfSize.y(); y += m_step.y()) {
        primitive->addVertex(Vector3(-m_halfSize.x(), 0, y), light);
        primitive->addVertex(Vector3(m_halfSize.x(), 0, y), light);
    }

    // on X axis
    for (Int32 x = -m_halfSize.x() ; x < 0 ; x += m_step.x()) {
        primitive->addVertex(Vector3(x, 0, -m_halfSize.y()), light);
        primitive->addVertex(Vector3(x, 0, m_halfSize.y()), light);
    }

    // origin line
    primitive->addVertex(Vector3(0, 0, -m_halfSize.y()), dark);
    primitive->addVertex(Vector3(0, 0, m_halfSize.y()), dark);

    for (Int32 x = m_step.x() ; x <= m_halfSize.x(); x += m_step.x()) {
        primitive->addVertex(Vector3(x, 0, -m_halfSize.y()), light);
        primitive->addVertex(Vector3(x, 0, m_halfSize.y()), light);
    }

    primitive->endDraw();
    primitive->modelView().pop();

    // restore
    scene->getContext()->setDefaultLineWidth();
    scene->getContext()->setAntiAliasing(aa);
}

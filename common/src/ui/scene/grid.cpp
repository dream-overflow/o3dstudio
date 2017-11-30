/**
 * @brief Common UI scene grid controller
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

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


Grid::Grid(const Point3f &pos, const Point2i &halfSize, const Point2i &step) :
    SceneUIElement(SCENE_UI_3D, True),
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

void Grid::directRendering(MasterScene *masterScene)
{
    Scene *scene = masterScene->scene();
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    // setup modelview
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix());

    scene->getContext()->setLineSize(2.0f);

    // on Y axis
    primitive->setColor(0.8f, 0.8f, 0.8f);
    for (UInt32 y = -m_halfSize.y() ; y < 0 ; y += m_step.y()) {

    }

    // origin line
    primitive->setColor(0.4f, 0.4f, 0.4f);
    // @todo

    primitive->setColor(0.8f, 0.8f, 0.8f);
    for (UInt32 y = m_step.y() ; y < m_halfSize.y(); y += m_step.y()) {

    }

    // on X axis
    primitive->setColor(0.8f, 0.8f, 0.8f);
    for (UInt32 y = -m_halfSize.y() ; y < 0 ; y += m_step.y()) {

    }

    // origin line
    primitive->setColor(0.4f, 0.4f, 0.4f);
    // @todo

    primitive->setColor(0.8f, 0.8f, 0.8f);
    for (UInt32 y = m_step.y() ; y < m_halfSize.y(); y += m_step.y()) {

    }

    scene->getContext()->setDefaultLineSize();
}

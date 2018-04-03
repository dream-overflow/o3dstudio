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
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>

#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/ui/scene/cameramanipulator.h"

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
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access(drawInfo);

    // in ortho camera with sided view grid always front
    if (masterScene->cameraManipulator()->cameraMode() == CameraManipulator::ORTHO &&
        masterScene->cameraManipulator()->cameraView() != CameraManipulator::VIEW_ANY) {
        primitive->modelView().identity();
        primitive->modelView().rotateX(-o3d::PI/2);
        primitive->modelView().translate(Vector3f(0, 0, 0));
    } else {
        // setup modelview
        primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix());
    }

    scene->getContext()->enableDepthTest();
    scene->getContext()->enableDepthWrite();

    primitive->modelView().push();

    Bool numSteps = 1;
    Point2i step = m_step;
    Box2i area(-m_halfSize, 2*m_halfSize);
    Point2i center(0, 0);

    if (masterScene->cameraManipulator()->cameraMode() == CameraManipulator::ORTHO &&
        masterScene->cameraManipulator()->cameraView() != CameraManipulator::VIEW_ANY) {
        // have sub-grid resolution
        numSteps = 2;

        // and cover all the viewport
        area.set(scene->getActiveCamera()->getLeft(),
                 scene->getActiveCamera()->getBottom(),
                 scene->getActiveCamera()->getRight() - scene->getActiveCamera()->getLeft(),
                 scene->getActiveCamera()->getTop() - scene->getActiveCamera()->getBottom());

        center.set((scene->getActiveCamera()->getLeft() + scene->getActiveCamera()->getRight()) * 0.5,
                   (scene->getActiveCamera()->getBottom() + scene->getActiveCamera()->getTop()) * 0.5);

        Int32 n = o3d::max(area.width() / 100, 1);
        Int32 p = 10 - (n % 10);

      //  printf("%i:%i, ", n, p); fflush(0);

        step.set(n*p, n*p);
    } else {
        // relative grid but should cover all the camera field of view
        primitive->modelView().translate(m_pos);
    }

    // Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_HINT_NICEST);
    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_MULTI_SAMPLE);

    scene->getContext()->setLineWidth(1.0f);

    // intensity
    primitive->setColor(1.f, 1.f, 1.f);

    primitive->beginDraw(P_LINES);

    Color light(0.85f, 0.85f, 0.85f);
    Color dark(0.6f, 0.6f, 0.6f);
    Color blue(0.15f, 0.15f, 0.7f);
    Color red(0.7f, 0.15f, 0.15f);

    // on X axis
    for (Int32 y = -step.y(); y >= area.y() ; y -= step.y()) {
       // for (Int32 n = 0; n < numSteps; ++n) {
            // @todo sub-grid and for the 4 parts
            primitive->addVertex(Vector3(area.x(), 0, y), light);
            primitive->addVertex(Vector3(area.x2(), 0, y), light);
       // }
    }

    // origin line
    primitive->addVertex(Vector3(area.x(), 0, 0), red);
    primitive->addVertex(Vector3(area.x2(), 0, 0), red);

    for (Int32 y = step.y(); y <= area.y2(); y += step.y()) {
        primitive->addVertex(Vector3(area.x(), 0, y), light);
        primitive->addVertex(Vector3(area.x2(), 0, y), light);
    }

    // on Z axis
    for (Int32 x = -step.x(); x >= area.x() ; x -= step.x()) {
        primitive->addVertex(Vector3(x, 0, area.y()), light);
        primitive->addVertex(Vector3(x, 0, area.y2()), light);
    }

    // origin line
    primitive->addVertex(Vector3(0, 0, area.y()), blue);
    primitive->addVertex(Vector3(0, 0, area.y2()), blue);

    for (Int32 x = step.x(); x <= area.x2(); x += step.x()) {
        primitive->addVertex(Vector3(x, 0, area.y()), light);
        primitive->addVertex(Vector3(x, 0, area.y2()), light);
    }

    primitive->endDraw();
    primitive->modelView().pop();

    // restore
    scene->getContext()->setDefaultLineWidth();
    scene->getContext()->setAntiAliasing(aa);
}

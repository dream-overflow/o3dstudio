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

    Int32 numSubDiv = 1;
    Point2f step(m_step.x(), m_step.y());
    Point2f subStep(m_step.x(), m_step.y());
    Box2f area(-m_halfSize, 2*m_halfSize);

    if (masterScene->cameraManipulator()->cameraMode() == CameraManipulator::ORTHO &&
        masterScene->cameraManipulator()->cameraView() != CameraManipulator::VIEW_ANY) {

        // have sub-grid resolution
        numSubDiv = 10;

//#if (0)
//        const Float divSize = numSubDiv;

//        // and cover all the viewport
//        area.set(scene->getActiveCamera()->getLeft(),
//                 scene->getActiveCamera()->getBottom(),
//                 scene->getActiveCamera()->getRight() - scene->getActiveCamera()->getLeft(),
//                 scene->getActiveCamera()->getTop() - scene->getActiveCamera()->getBottom());

//        const Float ratio = (Float)area.width() / (Float)masterScene->viewPort().width();

//        const Float minDivSize = ratio * 2000;
//        const Float maxDivSize = ratio * 5000;

//        Float n = o3d::max<Float>((20000 - area.width()) + area.width() / numSubDiv, 1);
//        if (n < minDivSize) {
//            n = maxDivSize;
//        } else if (n > maxDivSize) {
//            n = minDivSize;
//        }
//        step.set(n, n);

//        printf("%f:%i:%f, ", n, numSubDiv, ratio); fflush(0);
//#endif

        const Float divSize = numSubDiv;
        const Float minDivSize = numSubDiv / 0.8;
        const Float maxDivSize = numSubDiv * 1.8;

        // and cover all the viewport
        area.set(scene->getActiveCamera()->getLeft(),
                 scene->getActiveCamera()->getBottom(),
                 scene->getActiveCamera()->getRight() - scene->getActiveCamera()->getLeft(),
                 scene->getActiveCamera()->getTop() - scene->getActiveCamera()->getBottom());

        Float n = o3d::max<Float>((20000 - area.width()) + area.width() / numSubDiv, 1);
        printf("%f:%f:%f:%f, ", n,  area.width(), minDivSize, maxDivSize); fflush(0);

        if (area.width() > minDivSize) {
            while (n > (area.width() / maxDivSize)) {
                n -= (area.width() / divSize * 2);
            }

            while (n < (area.width() / minDivSize)) {
                n += (area.width() / divSize * 2);
            }
        }

        n = o3d::max(n, minDivSize);

        step.set(n, n);
        subStep = step / numSubDiv;
/*
        if (n >= minDivSize * 1.5) {
            numSubDiv = 10;
            subStep = step / numSubDiv;
        } else if (n < minDivSize * 1.5) {
            numSubDiv = 5;
            subStep = step / numSubDiv;
        } else if (n < minDivSize * 1.1) {
            numSubDiv = 0;
        }*/

        subStep = step / numSubDiv;

//        if (n >= 5 * numSubDiv * numSubDiv) {
//            subStep = step / numSubDiv;
//        } else if (n >= 3 * numSubDiv * numSubDiv) {
//            numSubDiv = 5;
//            subStep = step / numSubDiv;
//        } else {
//            numSubDiv = 0;
//        }
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
    Color dark(0.7f, 0.7f, 0.7f);
    Color blue(0.15f, 0.15f, 0.7f);
    Color red(0.7f, 0.15f, 0.15f);

    Int32 j;
    Float sx, sy;

    // on X axis
    for (Float y = -step.y(); y >= area.y() ; y -= step.y()) {
        primitive->addVertex(Vector3(area.x(), 0, y), light);
        primitive->addVertex(Vector3(area.x2(), 0, y), light);

        for (j = 1, sy = subStep.y(); j < numSubDiv; ++j, sy += subStep.y()) {
            primitive->addVertex(Vector3(area.x(), 0, y-sy), dark);
            primitive->addVertex(Vector3(area.x2(), 0, y-sy), dark);
        }
    }

    for (j = 1, sy = subStep.y(); j < numSubDiv; ++j, sy += subStep.y()) {
        primitive->addVertex(Vector3(area.x(), 0, 0-sy), dark);
        primitive->addVertex(Vector3(area.x2(), 0, 0-sy), dark);
    }

    // origin line
    primitive->addVertex(Vector3(area.x(), 0, 0), red);
    primitive->addVertex(Vector3(area.x2(), 0, 0), red);

    for (j = 1, sy = subStep.y(); j < numSubDiv; ++j, sy += subStep.y()) {
        primitive->addVertex(Vector3(area.x(), 0, 0+sy), dark);
        primitive->addVertex(Vector3(area.x2(), 0, 0+sy), dark);
    }

    for (Float y = step.y(); y <= area.y2(); y += step.y()) {
        primitive->addVertex(Vector3(area.x(), 0, y), light);
        primitive->addVertex(Vector3(area.x2(), 0, y), light);

        for (j = 1, sy = subStep.y(); j< numSubDiv; ++j, sy += subStep.y()) {
            primitive->addVertex(Vector3(area.x(), 0, y+sy), dark);
            primitive->addVertex(Vector3(area.x2(), 0, y+sy), dark);
        }
    }

    // on Z axis
    for (Float x = -step.x(); x >= area.x() ; x -= step.x()) {
        primitive->addVertex(Vector3(x, 0, area.y()), light);
        primitive->addVertex(Vector3(x, 0, area.y2()), light);

        for (j = 1, sx = subStep.x(); j < numSubDiv; ++j, sx += subStep.x()) {
            primitive->addVertex(Vector3(x-sx, 0, area.y()), dark);
            primitive->addVertex(Vector3(x-sx, 0, area.y2()), dark);
        }
    }

    for (j = 1, sx = subStep.x(); j < numSubDiv; ++j, sx += subStep.x()) {
        primitive->addVertex(Vector3(0-sx, 0, area.y()), dark);
        primitive->addVertex(Vector3(0-sx, 0, area.y2()), dark);
    }

    // origin line
    primitive->addVertex(Vector3(0, 0, area.y()), blue);
    primitive->addVertex(Vector3(0, 0, area.y2()), blue);

    for (j = 1, sx = subStep.x(); j < numSubDiv; ++j, sx += subStep.x()) {
        primitive->addVertex(Vector3(0+sx, 0, area.y()), dark);
        primitive->addVertex(Vector3(0+sx, 0, area.y2()), dark);
    }

    for (Float x = step.x(); x <= area.x2(); x += step.x()) {
        primitive->addVertex(Vector3(x, 0, area.y()), light);
        primitive->addVertex(Vector3(x, 0, area.y2()), light);

        for (j = 1, sx = subStep.x(); j < numSubDiv; ++j, sx += subStep.x()) {
            primitive->addVertex(Vector3(x+sx, 0, area.y()), dark);
            primitive->addVertex(Vector3(x+sx, 0, area.y2()), dark);
        }
    }

    primitive->endDraw();
    primitive->modelView().pop();

    // restore
    scene->getContext()->setDefaultLineWidth();
    scene->getContext()->setAntiAliasing(aa);
}

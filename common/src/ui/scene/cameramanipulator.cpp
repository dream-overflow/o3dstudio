/**
 * @brief Common UI scene camera manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @details
 */

#include "o3d/studio/common/ui/scene/cameramanipulator.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/object/primitivemanager.h>
#include <o3d/engine/object/camera.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


CameraManipulator::CameraManipulator(BaseObject *parent, const Point2f &pos, const Float scale) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_pos(pos),
    m_scale(scale)
{

}

CameraManipulator::~CameraManipulator()
{

}

void CameraManipulator::createToScene(MasterScene *)
{

}

void CameraManipulator::removeFromScene(MasterScene *)
{

}

void CameraManipulator::syncWithScene(MasterScene *)
{

}

void CameraManipulator::directRendering(Scene *scene)
{
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    // setup modelview
    primitive->modelView().set(scene->getActiveCamera()->getModelviewMatrix());

    Context::AntiAliasingMethod aa = scene->getContext()->setAntiAliasing(Context::AA_HINT_NICEST);

    scene->getContext()->setCullingMode(CULLING_BACK_FACE);
    scene->getContext()->disableDoubleSide();
    scene->getContext()->enableDepthTest();

    primitive->modelView().push();

    primitive->modelView().pop();

    // restore
    scene->getContext()->setAntiAliasing(aa);
}

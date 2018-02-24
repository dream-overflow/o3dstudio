/**
 * @brief Common UI scene hub manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-24
 * @details
 */

#include "o3d/studio/common/ui/scene/hubmanipulator.h"

#include <o3d/engine/context.h>
#include <o3d/engine/matrix.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/context.h>
#include <o3d/engine/primitive/primitivemanager.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scene.h>
#include <o3d/engine/drawinfo.h>

#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


HubManipulator::HubManipulator(BaseObject *parent, Hub* target, const Point3f &hitPos) :
    SceneUIElement(parent, SCENE_UI_3D, POST_DRAW, True),
    m_hitPos(hitPos),
    m_target(target)
{

}

HubManipulator::~HubManipulator()
{

}

void HubManipulator::createToScene(MasterScene *)
{

}

void HubManipulator::removeFromScene(MasterScene *)
{

}

void HubManipulator::syncWithScene(MasterScene *)
{

}

void HubManipulator::directRendering(DrawInfo &drawInfo, MasterScene *masterScene)
{
    // @todo a picking pass for manipulation
    if (drawInfo.pass != DrawInfo::AMBIENT_PASS) {
        return;
    }

    Scene *scene = masterScene->scene();
    PrimitiveAccess primitive = scene->getPrimitiveManager()->access();

    const Box2i &vp = scene->getContext()->getViewPort();
    const Float factor = 600.f;

    // @todo an highlighted bounding volume

    // @todo rotation ball

    // @todo translation axes

    // @todo scale axes ?? but could need a modifier

    // restore
    // scene->getContext()->setAntiAliasing(aa);
}

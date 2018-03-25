/**
 * @brief Common structural hub abstract class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-25
 * @details
 */

#include <o3d/engine/scene/scene.h>

#include <o3d/core/classfactory.h>

#include "o3d/studio/common/workspace/structuralhub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

using namespace o3d::studio::common;


StructuralHub::StructuralHub(const String &name, Entity *parent) :
    Hub(name, parent)
{

}

StructuralHub::~StructuralHub()
{

}

Entity::Role StructuralHub::role() const
{
    return ROLE_STRUCTURAL_HUB;
}

//const o3d::Matrix4 &StructuralHub::absoluteMatrix(MasterScene *masterScene) const
//{
//    return Matrix4::getIdentity();
//}

//o3d::Bool StructuralHub::isSpacialNode() const
//{
//    return True;
//}

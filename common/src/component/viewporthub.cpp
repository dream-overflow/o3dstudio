/**
 * @brief Common viewport component offering a viewport hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/camera.h>
#include <o3d/engine/scene/scenedrawer.h>
#include <o3d/engine/screenviewport.h>
#include <o3d/engine/feedbackviewport.h>
#include <o3d/engine/viewportmanager.h>

#include "o3d/studio/common/component/viewporthub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


ViewPortComponent::ViewPortComponent()
{
    m_name = "o3s::common::component::viewporthub";
    m_targetName = "o3s::common::hub::viewport";

    m_label = "Screen ViewPort Hub";
    m_icon = ":/icons/desktop_windows_black.svg";
}

ViewPortComponent::~ViewPortComponent()
{

}

void ViewPortComponent::setup()
{

}

Hub *ViewPortComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    ViewPortHub *viewportHub = new ViewPortHub(name, parent);
    viewportHub->setTypeRef(m_targetTypeRef);

    return viewportHub;
}

ViewPortHub::ViewPortHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_xpos(0.f),
    m_ypos(0.f),
    m_width(1.f),
    m_height(1.f),
    m_percent(True),
    m_isActive(True),
    m_camera(nullptr),
    m_drawer(nullptr)
{

}

ViewPortHub::~ViewPortHub()
{

}

void ViewPortHub::create()
{

}

o3d::Bool ViewPortHub::deletable() const
{
    // @todo instances
    return True;
}

o3d::Bool ViewPortHub::load()
{
    return Hub::load();
}

o3d::Bool ViewPortHub::save()
{
    return Hub::save();
}

o3d::Bool ViewPortHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

o3d::Bool ViewPortHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool ViewPortHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void ViewPortHub::createToScene(MasterScene *masterScene)
{
    // @todo
}

void ViewPortHub::removeFromScene(MasterScene *masterScene)
{
    Hub::removeFromScene(masterScene);

    // @todo
}

void ViewPortHub::syncWithScene(MasterScene *masterScene)
{
    // @todo
}

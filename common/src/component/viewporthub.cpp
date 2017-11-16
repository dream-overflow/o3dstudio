/**
 * @brief Common viewport component offering a viewport hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include "o3d/studio/common/component/viewporthub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


ViewportComponent::ViewportComponent()
{
    m_name = "o3s::common::component::viewporthub";
    m_targetName = "o3s::common::hub::viewport";
}

ViewportComponent::~ViewportComponent()
{

}

void ViewportComponent::setup()
{

}

Hub *ViewportComponent::buildHub(const QString &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    ViewportHub *viewportHub = new ViewportHub(name, parent);
    viewportHub->setProject(project);
    viewportHub->setTypeRef(m_targetTypeRef);

    if (parent->ref().light().baseTypeOf(TypeRef::project())) {
        project->addHub(viewportHub);
    } else if (parent->ref().light().baseTypeOf(TypeRef::hub())) {
        Hub *parentHub = static_cast<Hub*>(parent);
        parentHub->addHub(viewportHub);
    } else {
        delete viewportHub;
        return nullptr;
    }

    return viewportHub;
}

ViewportHub::ViewportHub(const QString &name, Entity *parent) :
    Hub(name, parent),
    m_pos(0.f, 0.f),
    m_size(1.f, 1.f),
    m_sizeInPercent(true)
{

}

ViewportHub::~ViewportHub()
{

}

void ViewportHub::create()
{

}

bool ViewportHub::load()
{
    return Hub::load();
}

bool ViewportHub::save()
{
    return Hub::save();
}

bool ViewportHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

bool ViewportHub::serializeContent(QDataStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return false;
    }

    return true;
}

bool ViewportHub::deserializeContent(QDataStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return false;
    }

    return true;
}

void ViewportHub::createToScene(MasterScene *masterScene)
{

}

void ViewportHub::removeFromScene(MasterScene *masterScene)
{

}

void ViewportHub::syncWithScene(MasterScene *masterScene)
{

}

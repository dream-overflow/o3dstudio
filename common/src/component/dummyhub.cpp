/**
 * @brief Common dummy component offering a dummy hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-16
 * @details
 */

#include "o3d/studio/common/component/dummyhub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


DummyHubComponent::DummyHubComponent() :
    Component()
{
    m_name = "o3s::common::component::dummyhub";
    m_targetName = "o3s::common::hub::dummy";

    m_label = "Dummy Hub";
    m_icon = ":/icons/call_split_black.svg";
}

DummyHubComponent::~DummyHubComponent()
{

}

void DummyHubComponent::setup()
{

}

Hub *DummyHubComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    DummyHub *dummyHub = new DummyHub(name, parent);
    dummyHub->setTypeRef(m_targetTypeRef);

    return dummyHub;
}

DummyHub::DummyHub(const String &name, Entity *parent) :
    StructuralHub(name, parent)
{

}

DummyHub::~DummyHub()
{

}

void DummyHub::create()
{
    Hub::create();
}

o3d::Bool DummyHub::deletable() const
{
    return True;
}

o3d::Bool DummyHub::load()
{
    return Hub::load();
}

o3d::Bool DummyHub::save()
{
    return Hub::save();
}

o3d::Bool DummyHub::exists() const
{
    return Entity::exists();
}

o3d::Bool DummyHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool DummyHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void DummyHub::createToScene(MasterScene */*masterScene*/)
{
    // nothing into the scene
}

void DummyHub::removeFromScene(MasterScene */*masterScene*/)
{
    // nothing into the scene
}

void DummyHub::syncWithScene(MasterScene */*masterScene*/)
{
    // nothing to do with the scene
}

const o3d::Transform *DummyHub::transform(o3d::UInt32 transformIndex) const
{
    return nullptr;
}

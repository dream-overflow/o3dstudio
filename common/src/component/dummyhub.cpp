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
}

DummyHubComponent::~DummyHubComponent()
{

}

void DummyHubComponent::setup()
{

}

Hub *DummyHubComponent::buildHub(const QString &name, Project *project, Entity *parent)
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

DummyHub::DummyHub(const QString &name, Entity *parent) :
    Hub(name, parent)
{

}

DummyHub::~DummyHub()
{

}

void DummyHub::create()
{

}

bool DummyHub::load()
{
    return Hub::load();
}

bool DummyHub::save()
{
    return Hub::save();
}

bool DummyHub::exists() const
{
    // @todo O3D
    return Entity::exists();
}

bool DummyHub::serializeContent(QDataStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return false;
    }

    return true;
}

bool DummyHub::deserializeContent(QDataStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return false;
    }

    return true;
}

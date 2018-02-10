/**
 * @brief Common root hub object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-08
 * @details
 */

#include "o3d/studio/common/workspace/roothub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/dummyhub.h"

using namespace o3d::studio::common;


RootHub::RootHub(const String &name, Project *parent) :
    Hub(name, parent)
{
    m_typeRef = TypeRef::hub();
    m_ref = ObjectRef(TypeRef::hub());
}

RootHub::~RootHub()
{
}

void RootHub::create()
{
    // like hub but without signal
    setDirty();

    project()->addEntity(this);
}

o3d::Bool RootHub::deletable() const
{
    return False;
}

void RootHub::createToScene(MasterScene *)
{
    // does nothing
}

void RootHub::syncWithScene(MasterScene *)
{
    // does nothing
}

void RootHub::removeFromScene(MasterScene *masterScene)
{
    // @todo does nothing
    // remove recursively all children, starting by leaves
    for (auto it1 = m_hubs.begin(); it1 != m_hubs.end(); ++it1) {
        it1->second->removeFromScene(masterScene);
    }
}

o3d::Bool RootHub::serializeContent(OutStream &stream) const
{
    Int32 num = m_hubs.size();
    stream << num;

    // @todo for now all in the project file, how if there is more 10^4 hubs ?
    // could be interseting to split per chunks of N hubs per file

    // children recursively, and by order
    const Hub *hub = nullptr;
    for (auto cit = m_hubsOrder.cbegin(); cit != m_hubsOrder.cend(); ++cit) {
        auto cit2 = m_hubs.find(*cit);
        hub = cit2->second;

        // uuid and type ref, for instanciation
        stream << hub->ref().uuid()
               << hub->ref().strong().typeName()
               << *hub;
    }

    return True;
}

o3d::Bool RootHub::deserializeContent(InStream &stream)
{
    Hub *hub = nullptr;

    // setup now
    create();

    String typeName;
    Uuid uuid;
    Int32 num = 0;
    stream >> num;

    ComponentRegistry &components = Application::instance()->components();
    Component *component = nullptr;

    for (Int32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        component = components.componentByTarget(typeName);
        if (component) {
            hub = component->buildHub("", m_project, this);
            hub->setRef(ObjectRef::buildRef(m_project, component->targetTypeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;
        } else {
            hub = new DummyHub("", this);
            hub->setRef(ObjectRef::buildRef(m_project, hub->typeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;

            // and pass the extra data
            // stream.skipRawData(hubSize...) // @todo
        }

        UInt64 hubId = hub->ref().light().id();

        m_hubs[hubId] = hub;
        m_hubsOrder.push_back(hubId);

        // called by create
        // project()->addEntity(hub);
    }

    return True;
}

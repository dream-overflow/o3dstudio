/**
 * @brief Common fragment object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;


Fragment::Fragment(const String &name, Entity *parent) :
    Entity(name, parent),
    m_masterScene(nullptr)
{
    m_typeRef = TypeRef::fragment();
    m_ref = ObjectRef(TypeRef::fragment());

    m_masterScene = new MasterScene(this);
}

Fragment::~Fragment()
{
    delete m_masterScene;
}

void Fragment::setProject(Project *project)
{
    m_parent = project;
}

Project *Fragment::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Fragment::project() const
{
    return static_cast<Project*>(m_parent);
}

MasterScene *Fragment::masterScene()
{
    return m_masterScene;
}

const MasterScene *Fragment::masterScene() const
{
    return m_masterScene;
}

void Fragment::setupMasterScene()
{
    if (m_masterScene) {
        m_masterScene->initialize();
    }
}

void Fragment::create()
{
    setDirty();
}

o3d::Bool Fragment::load()
{
    return True;
}

o3d::Bool Fragment::save()
{
    return True;
}

o3d::Bool Fragment::exists() const
{
    return Entity::exists();
}

void Fragment::linkToHub(Hub *hub)
{
    m_hub = hub;
    m_hubRef = hub->ref();

    setDirty();
}

Hub *Fragment::hub()
{
    return m_hub;
}

const Hub *Fragment::hub() const
{
    return m_hub;
}

o3d::Bool Fragment::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    stream << m_hubRef.uuid();

    return True;
}

o3d::Bool Fragment::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    Uuid uuid;
    stream >> uuid;

    if (!uuid.isNull()) {
        // @todo could create a entity map during import based on the index... optimized...
        m_hub = project()->findHub(uuid);

        if (m_hub) {
            m_hubRef = m_hub->ref();
        } else {
            return False;
        }
    }

    return True;
}

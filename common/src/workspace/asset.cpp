/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


Asset::Asset(const String &name, Entity *parent) :
    Entity(name, parent)
{
    m_typeRef = TypeRef::asset();
    m_ref = ObjectRef(TypeRef::asset());
}

Asset::~Asset()
{
//    Hub *hub = nullptr;
//    for (auto it ...
//        delete hub;
//    }
}

void Asset::setProject(Project *project)
{
    m_parent = project;
}

Project *Asset::project()
{
    return static_cast<Project*>(m_parent);
}

const Project *Asset::project() const
{
    return static_cast<Project*>(m_parent);
}

o3d::Int32 Asset::childIndexOf(Entity *entity) const
{
    // @todo
    return -1;
}

void Asset::create()
{
    setDirty();
}

o3d::Bool Asset::deletable() const
{
    return True;
}

o3d::Bool Asset::load()
{
    return True;
}

o3d::Bool Asset::save()
{
    return True;
}

o3d::Bool Asset::exists() const
{
    return Entity::exists();
}

o3d::Bool Asset::serializeContent(OutStream &stream) const
{
    if (!Entity::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool Asset::deserializeContent(InStream &stream)
{
    if (!Entity::deserializeContent(stream)) {
        return False;
    }

    return False;
}

void Asset::setCreator(const o3d::String &creator)
{
    m_creator = creator;
}

const o3d::String &Asset::creator() const
{
    return m_creator;
}

void Asset::setDescription(const o3d::String &descr)
{
    m_description = descr;
}

const o3d::String &Asset::description() const
{
    return m_description;
}

void Asset::setComment(const o3d::String &comment)
{
    m_comment = comment;
}

const o3d::String &Asset::comment()
{
    return m_comment;
}

void Asset::setVersion(o3d::UInt32 majorVers, o3d::UInt32 minorVers, o3d::UInt32 subVers)
{
    m_version[0] = majorVers;
    m_version[1] = minorVers;
    m_version[2] = subVers;
}

o3d::UInt32 Asset::version(o3d::UInt32 part) const
{
    if (part > 2) {
        O3D_ERROR(E_IndexOutOfRange("Version part index between 0..2"));
    }

    return m_version[part];
}

void Asset::setVersionStatus(Asset::VersionStatus status)
{
    m_versionStatus = status;
}

Asset::VersionStatus Asset::versionStatus() const
{
    return m_versionStatus;
}

void Asset::setRevision(o3d::UInt32 rev)
{
    m_revision = rev;
}

o3d::UInt32 Asset::revision() const
{
    return m_revision;
}

void Asset::setCreationTimestamp(const o3d::DateTime &date)
{
    m_creationTimestamp = date;
}

const o3d::DateTime &Asset::creationTimestamp() const
{
    return m_creationTimestamp;
}

/**
 * @brief Common object reference for public and protected usages
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

using namespace o3d::studio::common;

const TypeRef& TypeRef::TypeRef::project()
{
    static TypeRef projectTypeRef = TypeRef(PROJECT_TYPE_ID, PROJECT_TYPE_ID, PROJECT_TYPE_STRING);
    return projectTypeRef;
}

const TypeRef& TypeRef::TypeRef::hub()
{
    static TypeRef hubTypeRef = TypeRef(HUB_TYPE_ID, HUB_TYPE_ID, HUB_TYPE_STRING);
    return hubTypeRef;
}

const TypeRef& TypeRef::TypeRef::fragment()
{
    static TypeRef fragmentTypeRef = TypeRef(FRAGMENT_TYPE_ID, FRAGMENT_TYPE_ID, FRAGMENT_TYPE_STRING);
    return fragmentTypeRef;
}

const TypeRef& TypeRef::asset()
{
    static TypeRef projectTypeRef = TypeRef(ASSET_TYPE_ID, ASSET_TYPE_ID, ASSET_TYPE_STRING);
    return projectTypeRef;
}

const TypeRef &TypeRef::component()
{
    static TypeRef projectTypeRef = TypeRef(COMPONENT_TYPE_ID, COMPONENT_TYPE_ID, COMPONENT_TYPE_STRING);
    return projectTypeRef;
}

TypeRef::TypeRef() :
    m_id(0),
    m_name()
{

}

TypeRef::TypeRef(EntityBaseType baseType, UInt32 id, const String &name) :
    m_id((id << 4) | (baseType & 0x0f)),
    m_name(name)
{

}

TypeRef::TypeRef(const TypeRef &dup) :
    m_id(dup.m_id),
    m_name(dup.m_name)
{

}

TypeRef &TypeRef::operator=(const TypeRef &dup)
{
    m_id = dup.m_id;
    m_name = dup.m_name;

    return *this;
}

LightRef::LightRef() :
    m_typeId(0),
    m_id(0),
    m_projectId(0)
{

}

LightRef::LightRef(const LightRef &dup) :
    m_typeId(dup.m_typeId),
    m_id(dup.m_id),
    m_projectId(dup.m_projectId)
{

}

LightRef &LightRef::operator=(const LightRef &dup)
{
    m_typeId = dup.m_typeId;
    m_id = dup.m_id;
    m_projectId = dup.m_projectId;

    return *this;
}

StrongRef::StrongRef() :
    m_typeName(),
    m_uuid(),
    m_projectUuid()
{

}

StrongRef::StrongRef(const StrongRef &dup) :
    m_typeName(dup.m_typeName),
    m_uuid(dup.m_uuid),
    m_projectUuid(dup.m_projectUuid)
{

}

StrongRef &StrongRef::operator=(const StrongRef &dup)
{
    m_typeName = dup.m_typeName;
    m_uuid = dup.m_uuid;
    m_projectUuid = dup.m_projectUuid;

    return *this;
}

ObjectRef::ObjectRef()
{

}

ObjectRef::ObjectRef(const TypeRef &ref)
{
   m_lightRef.m_typeId = ref.id();
   m_strongRef.m_typeName = ref.name();
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = project->generateEntityId();
    ref.m_lightRef.m_projectId = (UInt32)project->ref().light().id();
    ref.m_lightRef.m_typeId = type.id();

    ref.m_strongRef.m_uuid = o3d::Uuid::uuid5();
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_typeName = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type, const Uuid &uuid)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = project->generateEntityId();
    ref.m_lightRef.m_projectId = (UInt32)project->ref().light().id();
    ref.m_lightRef.m_typeId = type.id();

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_typeName = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type, const Uuid &uuid, UInt64 id)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = id;
    ref.m_lightRef.m_projectId = (UInt32)project->ref().light().id();
    ref.m_lightRef.m_typeId = type.id();

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_typeName = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Workspace *workspace)
{
    if (!workspace) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = (UInt64)workspace->generateProjectId();
    ref.m_lightRef.m_typeId = TypeRef::makeTypeId(PROJECT_TYPE_ID);

    ref.m_strongRef.m_uuid = o3d::Uuid::uuid5();
    ref.m_strongRef.m_typeName = PROJECT_TYPE_STRING;

    return ref;
}

ObjectRef ObjectRef::buildRef(Workspace *workspace, const Uuid &uuid)
{
    if (!workspace) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = (UInt64)workspace->generateProjectId();
    ref.m_lightRef.m_typeId = TypeRef::makeTypeId(PROJECT_TYPE_ID);

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_typeName = PROJECT_TYPE_STRING;

    return ref;
}

ObjectRef::ObjectRef(const ObjectRef &dup) :
    m_lightRef(dup.m_lightRef),
    m_strongRef(dup.m_strongRef)
{

}

//Url ObjectRef::url() const
//{
//    String path = String("o3s://%1/%2")
//            .arg(m_strongRef.projectUuid().toRfc4122())
//            .arg(m_strongRef.uuid().toRfc4122());
//
//    return Url(path);
//}

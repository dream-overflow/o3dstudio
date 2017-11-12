/**
 * @brief Common object reference for public and protected usages
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/workspace.h"

using namespace o3d::studio::common;

TypeRef TypeRef::TypeRef::project()
{
    return TypeRef(PROJECT_TYPE_ID, PROJECT_TYPE_ID, PROJECT_TYPE_STRING);
}

TypeRef TypeRef::TypeRef::hub()
{
    return TypeRef(HUB_TYPE_ID, HUB_TYPE_ID, HUB_TYPE_STRING);
}

TypeRef TypeRef::TypeRef::fragment()
{
    return TypeRef(FRAGMENT_TYPE_ID, FRAGMENT_TYPE_ID, FRAGMENT_TYPE_STRING);
}

TypeRef TypeRef::asset()
{
    return TypeRef(ASSET_TYPE_ID, ASSET_TYPE_ID, ASSET_TYPE_STRING);
}

TypeRef::TypeRef() :
    m_baseType(0),
    m_id(0),
    m_name()
{

}

TypeRef::TypeRef(qint64 baseType, qint64 id, const QString &name) :
    m_baseType(baseType),
    m_id(id),
    m_name(name)
{

}

TypeRef::TypeRef(const TypeRef &dup) :
    m_baseType(dup.m_baseType),
    m_id(dup.m_id),
    m_name(dup.m_name)
{

}

TypeRef &TypeRef::operator=(const TypeRef &dup)
{
    m_baseType = dup.m_baseType;
    m_id = dup.m_id;
    m_name = dup.m_name;

    return *this;
}

LightRef::LightRef() :
    m_type(0),
    m_id(0),
    m_projectId(0)
{

}

LightRef::LightRef(const LightRef &dup) :
    m_type(dup.m_type),
    m_id(dup.m_id),
    m_projectId(dup.m_projectId)
{

}

LightRef &LightRef::operator=(const LightRef &dup)
{
    m_type = dup.m_type;
    m_id = dup.m_id;
    m_projectId = dup.m_projectId;

    return *this;
}

StrongRef::StrongRef() :
    m_type(),
    m_uuid(),
    m_projectUuid()
{

}

StrongRef::StrongRef(const StrongRef &dup) :
    m_type(dup.m_type),
    m_uuid(dup.m_uuid),
    m_projectUuid(dup.m_projectUuid)
{

}

StrongRef &StrongRef::operator=(const StrongRef &dup)
{
    m_type = dup.m_type;
    m_uuid = dup.m_uuid;
    m_projectUuid = dup.m_projectUuid;

    return *this;
}

ObjectRef::ObjectRef()
{

}

ObjectRef::ObjectRef(const TypeRef &ref)
{
   m_lightRef.m_type = ref.id();
   m_strongRef.m_type = ref.name();
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = project->generateId();
    ref.m_lightRef.m_projectId = project->ref().light().id();
    ref.m_lightRef.m_type = type.id();

    ref.m_strongRef.m_uuid = QUuid::createUuid();
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_type = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type, const QUuid &uuid)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = project->generateId();
    ref.m_lightRef.m_projectId = project->ref().light().id();
    ref.m_lightRef.m_type = type.id();

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_type = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Project *project, const TypeRef &type, const QUuid &uuid, qint64 id)
{
    if (!project) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = id;
    ref.m_lightRef.m_projectId = project->ref().light().id();
    ref.m_lightRef.m_type = type.id();

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_projectUuid = project->ref().strong().uuid();
    ref.m_strongRef.m_type = type.name();

    return ref;
}

ObjectRef ObjectRef::buildRef(Workspace *workspace)
{
    if (!workspace) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = workspace->generateId();
    ref.m_lightRef.m_type = PROJECT_TYPE_ID;

    ref.m_strongRef.m_uuid = QUuid::createUuid();
    ref.m_strongRef.m_type = PROJECT_TYPE_STRING;

    return ref;
}

ObjectRef ObjectRef::buildRef(Workspace *workspace, const QUuid &uuid)
{
    if (!workspace) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = workspace->generateId();
    ref.m_lightRef.m_type = PROJECT_TYPE_ID;

    ref.m_strongRef.m_uuid = uuid;
    ref.m_strongRef.m_type = PROJECT_TYPE_STRING;

    return ref;
}

ObjectRef::ObjectRef(const ObjectRef &dup) :
    m_lightRef(dup.m_lightRef),
    m_strongRef(dup.m_strongRef)
{

}

QUrl ObjectRef::url() const
{
    QString path = QString("o3s://%1/%2").arg(m_strongRef.projectUuid().toRfc4122().toStdString().c_str()).arg(m_lightRef.id());
    return QUrl(path);
}

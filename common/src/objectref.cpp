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
    return TypeRef(1, "o3s::project");
}

TypeRef TypeRef::TypeRef::fragment()
{
    return TypeRef(2, "o3s::fragment");
}

TypeRef TypeRef::TypeRef::hub()
{
    return TypeRef(3, "o3s::hub");
}

TypeRef::TypeRef() :
    m_id(0),
    m_name()
{

}

TypeRef::TypeRef(qint64 id, const QString &name) :
    m_id(id),
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

ObjectRef ObjectRef::buildRef(Workspace *workspace)
{
    if (!workspace) {
        throw;
    }

    ObjectRef ref;

    ref.m_lightRef.m_id = workspace->generateId();
    ref.m_lightRef.m_type = PROJECT_TYPE_ID;

    ref.m_strongRef.m_uuid = QUuid::createUuid();
    ref.m_strongRef.m_type = "o3s::project";

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
    ref.m_strongRef.m_type = "o3s::project";

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

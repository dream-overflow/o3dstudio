/**
 * @brief Common object reference for public and protected usages
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_OBJECTREF_H
#define _O3DS_COMMON_OBJECTREF_H

#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class Project;

/**
 * @brief Fixed entity base type identifier.
 */
enum EntityBaseType
{
    PROJECT_TYPE_ID = 1,
    HUB_TYPE_ID = 2,
    FRAGMENT_TYPE_ID = 3,
    ASSET_TYPE_ID = 4,
    MAX_TYPE_ID = ASSET_TYPE_ID
};

constexpr char PROJECT_TYPE_STRING[] = "o3s::project";
constexpr char HUB_TYPE_STRING[] = "o3s::hub";
constexpr char FRAGMENT_TYPE_STRING[] = "o3s::fragment";
constexpr char ASSET_TYPE_STRING[] = "o3s::asset";

/**
 * @brief Permit the identification of a type.
 */
class O3S_API TypeRef final
{
    friend class ObjectRef;

public:

    static const TypeRef& project();
    static const TypeRef& hub();
    static const TypeRef& fragment();
    static const TypeRef& asset();

    TypeRef();
    TypeRef(EntityBaseType baseType, quint32 id, const QString &name);
    TypeRef(const TypeRef &dup);

    TypeRef& operator= (const TypeRef &dup);

    inline bool isValid() const
    {
        return m_id > 0 && !m_name.isEmpty();
    }

    inline bool operator <(const TypeRef &to) const
    {
        return m_id < to.m_id;
    }

    inline bool operator <=(const TypeRef &to) const
    {
        return m_id <= to.m_id;
    }

    inline bool operator >(const TypeRef &to) const
    {
        return m_id > to.m_id;
    }

    inline bool operator >=(const TypeRef &to) const
    {
        return m_id >= to.m_id;
    }

    inline bool operator ==(const TypeRef &to) const
    {
        return m_id == to.m_id;
    }

    inline bool operator !=(const TypeRef &to) const
    {
        return m_id != to.m_id;
    }

    /**
     * @brief id of the TypeRef containing the baseType on the 4 LSB and the type id on the 28 MSB.
     * @return
     */
    inline quint32 id() const { return m_id; }

    /**
     * @brief id of the TypeRef of the base type. Similar as id for a base type.
     * @return
     */
    inline quint32 baseTypeId() const { return ((m_id & 0x0f) << 4) | (m_id & 0x0f); }

    /**
     * @brief id Unique per project 32 bits integer identifier.
     * This correspond to the 28 MSB of the id.
     * @return
     */
    inline quint32 typeId() const { return (m_id & ~0x0f) >> 4; }

    /**
     * @brief name Global reference of type of object.
     * @return
     */
    inline const QString& name() const { return m_name; }

private:

    quint32 m_id;
    QString m_name;

public:

    inline static quint32 makeTypeId(quint32 baseTypeId) {
        return ((baseTypeId & 0x0f) << 4) | (baseTypeId & 0x0f);
    }

    inline static quint32 hasBaseTypeId(quint32 id, quint32 baseTypeId) {
        // keep only the 4 LSB of the base type id
        return (id & 0x0f) == (baseTypeId & 0x0f);
    }

    friend uint qHash(const TypeRef &key, uint seed = 0)
    {
        return qHash(key.id(), seed);
    }
};


/**
 * @brief Permit the identification of an object internaly.
 * If projectId is > 0 it is an object inside of a project, else the id reference a project into the workspace.
 */
class O3S_API LightRef final
{
    friend class ObjectRef;

public:

    LightRef();
    LightRef(const LightRef &dup);

    LightRef& operator= (const LightRef &dup);

    inline bool isValid() const
    {
        if (m_projectId == 0) {
            return m_id > 0 && TypeRef::hasBaseTypeId(m_typeId, PROJECT_TYPE_ID);
        } else {
            return m_id > 0 && m_typeId > 0;
        }
    }

    inline bool operator <(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_typeId == to.m_typeId) {
                return m_id < to.m_id;
            } else {
                return m_typeId < to.m_typeId;
            }
        } else {
            return m_projectId < to.m_projectId;
        }
    }

    inline bool operator <=(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_typeId == to.m_typeId) {
                return m_id <= to.m_id;
            } else {
                return m_typeId <= to.m_typeId;
            }
        } else {
            return m_projectId <= to.m_projectId;
        }
    }

    inline bool operator >(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_typeId == to.m_typeId) {
                return m_id > to.m_id;
            } else {
                return m_typeId > to.m_typeId;
            }
        } else {
            return m_projectId > to.m_projectId;
        }
    }

    inline bool operator >=(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_typeId == to.m_typeId) {
                return m_id >= to.m_id;
            } else {
                return m_typeId >= to.m_typeId;
            }
        } else {
            return m_projectId >= to.m_projectId;
        }
    }

    inline bool operator ==(const LightRef &to) const
    {
        return m_projectId == to.m_projectId && m_typeId == to.m_typeId && m_id == to.m_id;
    }

    inline bool operator !=(const LightRef &to) const
    {
        return m_projectId != to.m_projectId || m_typeId != to.m_typeId || m_id != to.m_id;
    }

    /**
     * @brief Unique type of the object, registered value to the object registry.
     * @return
     */
    inline quint32 typeId() const { return m_typeId; }

    /**
     * @brief id Unique per project 64 bits integer identifier.
     * @return
     */
    inline quint64 id() const { return m_id; }

    /**
     * @brief projectId Registered integer value of the project into the current workspace.
     * @return
     */
    inline quint32 projectId() const { return m_projectId; }

    /**
     * @brief Check if the type of the reference inherit from a specific base type.
     * @return
     */
    inline bool baseTypeOf(const TypeRef &baseTypeRef) const
    {
        return TypeRef::hasBaseTypeId(m_typeId, baseTypeRef.id());
    }

    QString longId() const
    {
        return QString("%1:%2").arg(m_projectId).arg(m_id);
    }

private:

    quint32 m_typeId;
    quint64 m_id;
    quint32 m_projectId;

public:

    friend uint qHash(const LightRef &key, uint seed = 0)
    {
        return qHash(key.projectId()*key.id(), seed);
    }
};

/**
 * @brief Permit the strong identification of an object externaly
 */
class O3S_API StrongRef final
{
    friend class ObjectRef;

public:

    StrongRef();
    StrongRef(const StrongRef &dup);

    StrongRef& operator= (const StrongRef &dup);

    inline bool operator <(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_typeName == to.m_typeName) {
                return m_uuid < to.m_uuid;
            } else {
                return m_typeName < to.m_typeName;
            }
        } else {
            return m_projectUuid < to.m_projectUuid;
        }
    }

    inline bool operator <=(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_typeName == to.m_typeName) {
                return m_uuid <= to.m_uuid;
            } else {
                return m_typeName <= to.m_typeName;
            }
        } else {
            return m_projectUuid <= to.m_projectUuid;
        }
    }

    inline bool operator >(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_typeName == to.m_typeName) {
                return m_uuid > to.m_uuid;
            } else {
                return m_typeName > to.m_typeName;
            }
        } else {
            return m_projectUuid > to.m_projectUuid;
        }
    }

    inline bool operator >=(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_typeName == to.m_typeName) {
                return m_uuid >= to.m_uuid;
            } else {
                return m_typeName >= to.m_typeName;
            }
        } else {
            return m_projectUuid >= to.m_projectUuid;
        }
    }

    inline bool operator ==(const StrongRef &to) const
    {
        return m_projectUuid == to.m_projectUuid && m_typeName == to.m_typeName && m_uuid == to.m_uuid;
    }

    inline bool operator !=(const StrongRef &to) const
    {
        return m_projectUuid != to.m_projectUuid || m_typeName != to.m_typeName || m_uuid != to.m_uuid;
    }

    /**
     * @brief Unique type of the object, registered string to the object registry.
     * @return
     */
    inline const QString& typeName() const { return m_typeName; }

    /**
     * @brief UUID of the object
     * @return
     */
    inline const QUuid& uuid() const { return m_uuid; }

    /**
     * @brief UUID for the owner project
     * @return
     */
    inline const QUuid& projectUuid() const { return m_projectUuid; }

    QString longUuid() const
    {
        return QString("%1:%2").arg(m_projectUuid.toString()).arg(m_uuid.toString());
    }

private:

    QString m_typeName;
    QUuid m_uuid;
    QUuid m_projectUuid;
};

/**
 * @brief Permit the identification of an object internaly or externaly with two types of identificiations :
 *  - light for internal, more short, but robust within a project/workspace
 *  - strong for external, bigger, but works in all cases with third parts
 */
class O3S_API ObjectRef final
{
public:

    ObjectRef();

    ObjectRef(const TypeRef &ref);

    /**
     * @brief buildRef Build a new reference for an object into the project
     * @param project
     * @param type Reference on the type of the object to reference
     * @return
     */
    static ObjectRef buildRef(Project *project, const TypeRef &type);

    /**
     * @brief buildRef Build a new reference for an object into the project and force uuid values
     * @param project
     * @param type Reference on the type of the object to reference
     * @return
     */
    static ObjectRef buildRef(Project *project, const TypeRef &type, const QUuid &uuid);

    /**
     * @brief buildRef Build a new reference for an object into the project and force uuid and id values
     * @param project
     * @param type Reference on the type of the object to reference
     * @return
     */
    static ObjectRef buildRef(Project *project, const TypeRef &type, const QUuid &uuid, quint64 id);

    /**
     * @brief buildRef Build a new reference for a project into the workspace
     * @param workspace
     * @return
     */
    static ObjectRef buildRef(Workspace *workspace);

    /**
     * @brief buildRef Build a new reference for an object into the project
     * @param project
     * @param uuid The strong reference project UUID
     * @return
     */
    static ObjectRef buildRef(Workspace *workspace, const QUuid &uuid);

    ObjectRef(const ObjectRef &dup);

    inline const LightRef& light() const { return m_lightRef; }
    inline const StrongRef& strong() const { return m_strongRef; }

    inline const QUuid &uuid() const { return m_strongRef.uuid(); }
    QUrl url() const;

    inline operator LightRef() { return m_lightRef; }
    inline operator StrongRef() { return m_strongRef; }

private:

    LightRef m_lightRef;
    StrongRef m_strongRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

Q_DECLARE_METATYPE(o3d::studio::common::LightRef)

#endif // _O3DS_COMMON_ELEMENT_H

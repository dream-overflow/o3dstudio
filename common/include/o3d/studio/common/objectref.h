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

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class Project;

/**
 * @brief Permit the identification of a type.
 */
class TypeRef final
{
    friend class ObjectRef;

public:

    TypeRef();
    TypeRef(const TypeRef &dup);

    TypeRef& operator= (const TypeRef &dup);

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
     * @brief id Unique per project 64 bits integer identifier.
     * @return
     */
    inline qint64 id() const { return m_id; }

    /**
     * @brief name Global reference of type of object.
     * @return
     */
    inline const QString& name() const { return m_name; }

private:

    qint64 m_id;
    QString m_name;
};

/**
 * @brief Permit the identification of an object internaly.
 * If projectId is > 0 it is an object inside of a project, else the id reference a project into the workspace.
 */
class LightRef final
{
    friend class ObjectRef;

public:

    LightRef();
    LightRef(const LightRef &dup);

    LightRef& operator= (const LightRef &dup);

    inline bool operator <(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_type == to.m_type) {
                return m_id < to.m_id;
            } else {
                return m_type < to.m_type;
            }
        } else {
            return m_projectId < to.m_projectId;
        }
    }

    inline bool operator <=(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_type == to.m_type) {
                return m_id <= to.m_id;
            } else {
                return m_type <= to.m_type;
            }
        } else {
            return m_projectId <= to.m_projectId;
        }
    }

    inline bool operator >(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_type == to.m_type) {
                return m_id > to.m_id;
            } else {
                return m_type > to.m_type;
            }
        } else {
            return m_projectId > to.m_projectId;
        }
    }

    inline bool operator >=(const LightRef &to) const
    {
        if (m_projectId == to.m_projectId) {
            if (m_type == to.m_type) {
                return m_id >= to.m_id;
            } else {
                return m_type >= to.m_type;
            }
        } else {
            return m_projectId >= to.m_projectId;
        }
    }

    inline bool operator ==(const LightRef &to) const
    {
        return m_projectId == to.m_projectId && m_type == to.m_type && m_id == to.m_id;
    }

    inline bool operator !=(const LightRef &to) const
    {
        return m_projectId != to.m_projectId || m_type != to.m_type || m_id != to.m_id;
    }

    /**
     * @brief Unique type of the object, registered value to the object registry.
     * @return
     */
    inline int type() const { return m_type; }

    /**
     * @brief id Unique per project 64 bits integer identifier.
     * @return
     */
    inline qint64 id() const { return m_id; }

    /**
     * @brief projectId Registered integer value of the project into the current workspace.
     * @return
     */
    inline qint64 projectId() const { return m_projectId; }

    QString longId() const
    {
        return QString("%1:%2").arg(m_projectId).arg(m_id);
    }

private:

    int m_type;
    qint64 m_id;
    qint64 m_projectId;
};

/**
 * @brief Permit the strong identification of an object externaly
 */
class StrongRef final
{
    friend class ObjectRef;

public:

    StrongRef();
    StrongRef(const StrongRef &dup);

    StrongRef& operator= (const StrongRef &dup);

    inline bool operator <(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_type == to.m_type) {
                return m_uuid < to.m_uuid;
            } else {
                return m_type < to.m_type;
            }
        } else {
            return m_projectUuid < to.m_projectUuid;
        }
    }

    inline bool operator <=(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_type == to.m_type) {
                return m_uuid <= to.m_uuid;
            } else {
                return m_type <= to.m_type;
            }
        } else {
            return m_projectUuid <= to.m_projectUuid;
        }
    }

    inline bool operator >(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_type == to.m_type) {
                return m_uuid > to.m_uuid;
            } else {
                return m_type > to.m_type;
            }
        } else {
            return m_projectUuid > to.m_projectUuid;
        }
    }

    inline bool operator >=(const StrongRef &to) const
    {
        if (m_projectUuid == to.m_projectUuid) {
            if (m_type == to.m_type) {
                return m_uuid >= to.m_uuid;
            } else {
                return m_type >= to.m_type;
            }
        } else {
            return m_projectUuid >= to.m_projectUuid;
        }
    }

    inline bool operator ==(const StrongRef &to) const
    {
        return m_projectUuid == to.m_projectUuid && m_type == to.m_type && m_uuid == to.m_uuid;
    }

    inline bool operator !=(const StrongRef &to) const
    {
        return m_projectUuid != to.m_projectUuid || m_type != to.m_type || m_uuid != to.m_uuid;
    }

    /**
     * @brief Unique type of the object, registered string to the object registry.
     * @return
     */
    inline const QString& type() const { return m_type; }

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

    QString m_type;
    QUuid m_uuid;
    QUuid m_projectUuid;
};

/**
 * @brief Permit the identification of an object internaly or externaly with two types of identificiations :
 *  - light for internal, more short, but robust within a project/workspace
 *  - strong for external, bigger, but works in all cases with third parts
 */
class ObjectRef final
{
public:

    ObjectRef();

    /**
     * @brief buildRef Build a new reference for an object into the project
     * @param project
     * @param type Reference on the type of the object to reference
     * @return
     */
    static ObjectRef buildRef(Project *project, const TypeRef &type);

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

#endif // _O3DS_COMMON_ELEMENT_H

/**
 * @brief Common object reference for public and protected usages
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_OBJECTREF_H
#define _O3DS_COMMON_OBJECTREF_H

#include <o3d/core/uuid.h>

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
    MIN_TYPE_ID = PROJECT_TYPE_ID,
    HUB_TYPE_ID = 2,
    FRAGMENT_TYPE_ID = 3,
    ASSET_TYPE_ID = 4,
    COMPONENT_TYPE_ID = 5,
    MAX_TYPE_ID = COMPONENT_TYPE_ID
};

constexpr char PROJECT_TYPE_STRING[] = "o3s::project";
constexpr char HUB_TYPE_STRING[] = "o3s::hub";
constexpr char FRAGMENT_TYPE_STRING[] = "o3s::fragment";
constexpr char ASSET_TYPE_STRING[] = "o3s::asset";
constexpr char COMPONENT_TYPE_STRING[] = "o3s::component";

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
    static const TypeRef& component();

    TypeRef();
    TypeRef(EntityBaseType baseType, UInt32 id, const String &name);
    TypeRef(const TypeRef &dup);

    TypeRef& operator= (const TypeRef &dup);

    inline Bool isValid() const
    {
        return m_id > 0 && !m_name.isEmpty();
    }

    inline Bool operator <(const TypeRef &to) const
    {
        return m_id < to.m_id;
    }

    inline Bool operator <=(const TypeRef &to) const
    {
        return m_id <= to.m_id;
    }

    inline Bool operator >(const TypeRef &to) const
    {
        return m_id > to.m_id;
    }

    inline Bool operator >=(const TypeRef &to) const
    {
        return m_id >= to.m_id;
    }

    inline Bool operator ==(const TypeRef &to) const
    {
        return m_id == to.m_id;
    }

    inline Bool operator !=(const TypeRef &to) const
    {
        return m_id != to.m_id;
    }

    /**
     * @brief id of the TypeRef containing the baseType on the 4 LSB and the type id on the 28 MSB.
     * @return
     */
    inline UInt32 id() const { return m_id; }

    /**
     * @brief id of the TypeRef of the base type. Similar as id for a base type.
     * @return
     */
    inline UInt32 baseTypeId() const { return ((m_id & 0x0f) << 4) | (m_id & 0x0f); }

    /**
     * @brief id Unique per project 32 bits integer identifier.
     * This correspond to the 28 MSB of the id.
     * @return
     */
    inline UInt32 typeId() const { return (m_id & ~0x0f) >> 4; }

    /**
     * @brief name Global reference of type of object.
     * @return
     */
    inline const String& name() const { return m_name; }

private:

    UInt32 m_id;
    String m_name;

public:

    inline static UInt32 makeTypeId(UInt32 baseTypeId) {
        return ((baseTypeId & 0x0f) << 4) | (baseTypeId & 0x0f);
    }

    inline static UInt32 hasBaseTypeId(UInt32 id, UInt32 baseTypeId) {
        // keep only the 4 LSB of the base type id
        return (id & 0x0f) == (baseTypeId & 0x0f);
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

    inline Bool operator <(const LightRef &to) const
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

    inline Bool operator <=(const LightRef &to) const
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

    inline Bool operator >(const LightRef &to) const
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

    inline Bool operator >=(const LightRef &to) const
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

    inline Bool operator ==(const LightRef &to) const
    {
        return m_projectId == to.m_projectId && m_typeId == to.m_typeId && m_id == to.m_id;
    }

    inline Bool operator !=(const LightRef &to) const
    {
        return m_projectId != to.m_projectId || m_typeId != to.m_typeId || m_id != to.m_id;
    }

    /**
     * @brief Unique type of the object, registered value to the object registry.
     * @return
     */
    inline UInt32 typeId() const { return m_typeId; }

    /**
     * @brief id Unique per project 64 bits integer identifier.
     * @return
     */
    inline UInt64 id() const { return m_id; }

    /**
     * @brief projectId Registered integer value of the project into the current workspace.
     * @return
     */
    inline UInt32 projectId() const { return m_projectId; }

    /**
     * @brief Check if the type of the reference inherit from a specific base type.
     * @return
     */
    inline Bool baseTypeOf(const TypeRef &baseTypeRef) const
    {
        return TypeRef::hasBaseTypeId(m_typeId, baseTypeRef.id());
    }

    String longId() const
    {
        return String::print("%i:%i", m_projectId, m_id);
    }

private:

    UInt32 m_typeId;
    UInt64 m_id;
    UInt32 m_projectId;
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

    inline Bool operator <=(const StrongRef &to) const
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

    inline Bool operator >(const StrongRef &to) const
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

    inline Bool operator >=(const StrongRef &to) const
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

    inline Bool operator ==(const StrongRef &to) const
    {
        return m_projectUuid == to.m_projectUuid && m_typeName == to.m_typeName && m_uuid == to.m_uuid;
    }

    inline Bool operator !=(const StrongRef &to) const
    {
        return m_projectUuid != to.m_projectUuid || m_typeName != to.m_typeName || m_uuid != to.m_uuid;
    }

    /**
     * @brief Unique type of the object, registered string to the object registry.
     * @return
     */
    inline const String& typeName() const { return m_typeName; }

    /**
     * @brief UUID of the object
     * @return
     */
    inline const Uuid& uuid() const { return m_uuid; }

    /**
     * @brief UUID for the owner project
     * @return
     */
    inline const Uuid& projectUuid() const { return m_projectUuid; }

    String longUuid() const
    {
        return String::print("%s:%s", m_projectUuid.toString(), m_uuid.toString());
    }

private:

    String m_typeName;
    Uuid m_uuid;
    Uuid m_projectUuid;
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
    static ObjectRef buildRef(Project *project, const TypeRef &type, const Uuid &uuid);

    /**
     * @brief buildRef Build a new reference for an object into the project and force uuid and id values
     * @param project
     * @param type Reference on the type of the object to reference
     * @return
     */
    static ObjectRef buildRef(Project *project, const TypeRef &type, const Uuid &uuid, UInt64 id);

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
    static ObjectRef buildRef(Workspace *workspace, const Uuid &uuid);

    ObjectRef(const ObjectRef &dup);

    inline const LightRef& light() const { return m_lightRef; }
    inline const StrongRef& strong() const { return m_strongRef; }

    inline const Uuid &uuid() const { return m_strongRef.uuid(); }
    // Url url() const; @todo later

    inline operator LightRef() { return m_lightRef; }
    inline operator StrongRef() { return m_strongRef; }

private:

    LightRef m_lightRef;
    StrongRef m_strongRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

namespace std {

template<>
struct hash<o3d::studio::common::TypeRef>
{
    enum
    {
        bucket_size = 8,
        min_buckets = 16
    };

    bool operator() (const o3d::studio::common::TypeRef &s1, const o3d::studio::common::TypeRef &s2) const
    {
        if (s1.id() < s2.id()) {
            return -1;
        } else if (s1.id() > s2.id()) {
            return 1;
        } else {
            return 0;
        }
    }

    size_t operator() (const o3d::studio::common::TypeRef &s1) const
    {
        return s1.id();
    }
};

template<>
struct hash<o3d::studio::common::LightRef>
{
    enum
    {
        bucket_size = 8,
        min_buckets = 16
    };

    bool operator() (const o3d::studio::common::LightRef &s1, const o3d::studio::common::LightRef &s2) const
    {
        if (s1 < s2) {
            return -1;
        } else if (s1 > s2) {
            return 1;
        } else {
            return 0;
        }
    }

    size_t operator() (const o3d::studio::common::LightRef &s1) const
    {
        return s1.id() * s1.projectId();
    }
};

}

#endif // _O3DS_COMMON_ELEMENT_H

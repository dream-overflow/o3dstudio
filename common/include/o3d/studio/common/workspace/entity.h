/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_ENTITY_H
#define _O3DS_COMMON_ENTITY_H

#include <QtCore/QString>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>
#include <QtCore/QBitArray>
#include <QtCore/QDataStream>

#include "../exception.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Entity abstract class
 */
class O3S_API Entity
{
public:

    enum Capacity
    {
        STATE_CHANGED = 32
    };

    explicit Entity(const QString &name, Entity *parent = nullptr);
    virtual ~Entity();

    Entity* parent();
    const Entity* parent() const;

    void setName(const QString& name);
    const QString& name() const;

    void setRef(const ObjectRef &ref);
    const ObjectRef& ref() const;

    void setTypeRef(const TypeRef &typeRef);
    const TypeRef& typeRef() const;

    virtual void create() = 0;

    virtual bool load() = 0;
    virtual bool save() = 0;

    /**
     * @brief Project related to the entity.
     */
    virtual Project* project() = 0;

    /**
     * @brief Project related to the entity (const version).
     */
    virtual const Project* project() const = 0;

    /**
     * @brief By default it exists if the reference point to a project.
     */
    virtual bool exists() const;

    /**
     * @brief Has changes to saved since last save(). Default ready the STATE_CHANGED flag.
     */
    virtual bool hasChanges() const;

    /**
     * @brief Serialize the entity content.
     */
    virtual bool serializeContent(QDataStream &stream) const;

    /**
     * @brief Deserialize the entity content.
     */
    virtual bool deserializeContent(QDataStream &stream);

protected:

    TypeRef m_typeRef;   //!< Related object reference type

    QString m_name;      //!< Project display name
    ObjectRef m_ref;

    Entity *m_parent;

    QBitArray m_capacities{64};  //!< Flags and capacities

    inline void setDirty() { m_capacities.setBit(STATE_CHANGED); }
    inline void setClean() { m_capacities.clearBit(STATE_CHANGED); }
    inline bool isDirty() const { return m_capacities.testBit(STATE_CHANGED); }

public:

    friend QDataStream& operator<<(QDataStream& stream, const Entity &entity)
    {
        entity.serializeContent(stream);
        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, Entity &entity)
    {
        entity.deserializeContent(stream);
        return stream;
    }
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ENTITY_H

/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_ENTITY_H
#define _O3DS_COMMON_ENTITY_H

#include <QtCore/QString>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

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

    explicit Entity(const QString &name, Entity *parent = nullptr);
    virtual ~Entity();

    virtual void create() = 0;

    void setRef(const ObjectRef &ref);

    Entity* parent();
    const Entity* parent() const;

    const QString& name() const;
    const ObjectRef& ref() const;

    const TypeRef& typeRef() const;

    virtual bool load() = 0;
    virtual bool save() = 0;

    virtual bool exists() const = 0;
    virtual bool hasChanges() = 0;

protected:

    TypeRef m_typeRef;   //!< Related object reference type

    QString m_name;      //!< Project display name
    ObjectRef m_ref;

    Entity *m_parent;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ENTITY_H

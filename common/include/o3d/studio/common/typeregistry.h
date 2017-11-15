/**
 * @brief Common type(ref) registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_TYPEREGISTRY_H
#define _O3DS_COMMON_TYPEREGISTRY_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QStringList>

#include "global.h"
#include "objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API TypeRegistry : public QObject
{
    Q_OBJECT

public:

    TypeRegistry(QObject *parent = nullptr);
    virtual ~TypeRegistry();

    bool registerType(EntityBaseType baseType, const QString &name);

    bool unregisterType(const QString &name);
    bool unregisterType(quint32 id);
    bool unregisterType(const TypeRef &typeRef);

    TypeRef typeRef(const QString &name) const;

    /**
     * @brief Return the base type for a type ref
     */
    TypeRef baseTypeRef(const TypeRef &typeRef) const;

    /**
     * @brief Return the base type for a type ref id
     */
    TypeRef baseTypeRef(quint32 id) const;

signals:

    void typeRegistered(TypeRef typeRef);

protected:

    quint32 m_nextId;

    QMap<QString, TypeRef> m_typeRefs;
    QMap<quint32, TypeRef> m_typeRefsById;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TYPEREGISTRY_H

/**
 * @brief Common builder registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#ifndef _O3DS_COMMON_BUILDERREGISTRY_H
#define _O3DS_COMMON_BUILDERREGISTRY_H

#include <QtCore/QCoreApplication>

#include <map>

#include <o3d/core/baseobject.h>

#include "../exception.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Builder;

class O3S_API BuilderRegistry : public BaseObject
{
    Q_DECLARE_TR_FUNCTIONS(BuilderRegistry)

public:

    BuilderRegistry(BaseObject *parent = nullptr);
    virtual ~BuilderRegistry();

    /**
     * @brief Register the default builders.
     */
    void initialize();

    /**
     * @brief Register a builder, that comes from a plugin or from the common.
     * @param builder Valid builder and must have a unique signature.
     */
    void registerBuilder(Builder *builder);

    Bool unregisterBuilder(const String &name);
    Bool unregisterBuilder(UInt64 id);
    Bool unregisterBuilder(const TypeRef &ref);

    Builder* builder(const String &name);
    const Builder* builder(const String &name) const;

    Builder* builder(const TypeRef &ref);
    const Builder* builder(const TypeRef &ref) const;

    std::list<const Builder*> builderList() const;

public /*signals*/:

    Signal<String> onBuilderRegistered{this};
    Signal<String> onBuilderUnregistered{this};

protected:

    std::map<String, Builder*> m_builders;
    std::map<UInt64, Builder*> m_buildersById;
};

/**
 * @brief The BuilderException class
 */
class E_BuilderException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_BuilderException)

    //! Ctor
    E_BuilderException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_BuilderException, "Objective-3D Studio builder exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_BUILDERREGISTRY_H

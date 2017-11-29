/**
 * @brief Common component registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENTREGISTRY_H
#define _O3DS_COMMON_COMPONENTREGISTRY_H

#include <QtCore/QCoreApplication>

#include <map>

#include <o3d/core/baseobject.h>

#include "../objectref.h"
#include "../exception.h"

namespace o3d {
namespace studio {
namespace common {

class Component;

class O3S_API ComponentRegistry : public BaseObject
{
    Q_DECLARE_TR_FUNCTIONS(ComponentRegistry)

public:

    ComponentRegistry(BaseObject *parent = nullptr);
    virtual ~ComponentRegistry();

    /**
     * @brief Register the default common components.
     */
    void initialize();

    /**
     * @brief Register a component, that comes from a plugin or from the common.
     * @param component Valid component and must have a unique signature.
     */
    void registerComponent(Component *component);

    Bool unregisterComponent(const String &name);
    Bool unregisterComponent(UInt64 id);
    Bool unregisterComponent(const TypeRef &ref);

    Component* component(const String &name);
    const Component* component(const String &name) const;

    Component* component(const TypeRef &ref);
    const Component* component(const TypeRef &ref) const;

    Component* componentByTarget(const String &name);
    const Component* componentByTarget(const String &name) const;

    std::list<const Component*> componentList() const;

public /*signals*/:

    Signal<String> onComponentRegistered{this};
    Signal<String> onComponentUnregistered{this};

protected:

    std::map<String, Component*> m_components;
    std::map<UInt64, Component*> m_componentsById;
    std::map<String, Component*> m_componentsByTargetName;
};

/**
 * @brief The ComponentException class
 */
class E_ComponentException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_ComponentException)

    //! Ctor
    E_ComponentException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_ComponentException, "Objective-3D Studio component exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENTREGISTRY_H

/**
 * @brief Common component registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENTREGISTRY_H
#define _O3DS_COMMON_COMPONENTREGISTRY_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include "../objectref.h"
#include "../exception.h"

namespace o3d {
namespace studio {
namespace common {

class Component;

class O3S_API ComponentRegistry : public QObject
{
    Q_OBJECT

public:

    ComponentRegistry(QObject *parent = nullptr);
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

    bool unregisterComponent(const QString &name);
    bool unregisterComponent(quint64 id);
    bool unregisterComponent(const TypeRef &ref);

    Component* component(const QString &name);
    const Component* component(const QString &name) const;

    Component* component(const TypeRef &ref);
    const Component* component(const TypeRef &ref) const;

    Component* componentByTarget(const QString &name);
    const Component* componentByTarget(const QString &name) const;

signals:

    void onComponentRegistered(QString name);
    void onComponentUnregistered(QString name);

protected:

    QMap<QString, Component*> m_components;
    QMap<quint64, Component*> m_componentsById;
    QMap<QString, Component*> m_componentsByTargetName;
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

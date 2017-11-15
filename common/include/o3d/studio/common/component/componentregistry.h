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

class O3S_API Component;

class ComponentRegistry : public QObject
{
    Q_OBJECT

public:

    ComponentRegistry(QObject *parent = nullptr);
    virtual ~ComponentRegistry();

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

signals:

    void onComponentRegistered(QString name);
    void onComponentUnregistered(QString name);

protected:

    QMap<QString, Component*> m_components;
    QMap<quint64, Component*> m_componentsById;
};

/**
 * @brief The ComponentException class
 */
class ComponentException : public BaseException
{
public:

    ComponentException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENTREGISTRY_H

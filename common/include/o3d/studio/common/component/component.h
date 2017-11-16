/**
 * @brief Common component base abstract class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENT_H
#define _O3DS_COMMON_COMPONENT_H

#include <QtWidgets/QWidget>
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

class O3S_API Component
{
public:

    Component();
    virtual ~Component();

    void setTypeRef(const TypeRef &typeRef);
    const TypeRef& typeRef() const;

    /**
     * @brief Unique component type string name.
     */
    const QString& name() const;

    /**
     * @brief Unique hub type string name.
     */
    const QString& targetName() const;

    /**
     * @brief Set the target type ref.
     */
    void setTargetTypeRef(const TypeRef& typeRef);

    /**
     * @brief Target type ref.
     */
    const TypeRef& targetTypeRef() const;

    // @todo soit ComponentUi object or directly label, icon.. action ?
    // const QString& label() const;

    /**
     * @brief Setup the component once registered.
     */
    virtual void setup() = 0;

    /**
     * @brief Create a new instance of the hub related to this component.
     * @param project Project owner
     * @param parent Parent of the new hub
     * @return
     */
    virtual Hub* buildHub(const QString &name, Project *project, Entity *parent) = 0;

protected:

    TypeRef m_typeRef;
    QString m_name;

    TypeRef m_targetTypeRef;
    QString m_targetName;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENT_H

/**
 * @brief Common component base abstract class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENT_H
#define _O3DS_COMMON_COMPONENT_H

#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

/**
 * @brief The Component abstract class. A component offers the way to instanciate a
 * new hub for a project.
 */
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
    const String& name() const;

    /**
     * @brief Unique hub type string name.
     */
    const String &targetName() const;

    /**
     * @brief Set the target type ref.
     */
    void setTargetTypeRef(const TypeRef& typeRef);

    /**
     * @brief Target type ref.
     */
    const TypeRef& targetTypeRef() const;

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
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) = 0;

    //
    // ui part
    //

    /**
     * @brief Display label and tooltip.
     */
    const String& label() const;

    /**
     * @brief Resource name of the icon.
     */
    const String& icon() const;

    // @todo a ComponentUi object when acting

protected:

    TypeRef m_typeRef;
    String m_name;

    TypeRef m_targetTypeRef;
    String m_targetName;

    String m_label;
    String m_icon;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENT_H

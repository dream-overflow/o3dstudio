/**
 * @brief Common entity object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_ENTITY_H
#define _O3DS_COMMON_ENTITY_H

#include <o3d/core/templatebitset.h>

#include "../exception.h"
#include "../objectref.h"

#include <o3d/core/instream.h>
#include <o3d/core/outstream.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Entity abstract class
 */
class O3S_API Entity
{
public:

    enum Changes
    {
        PRESENTATION_CHANGED = 0,   //!< Presentation changed
        DISPLAY_CHANGED = 1         //!< Display changed
    };

    enum Capacity
    {
        STATE_CHANGED = 32,      //!< State (properties...) changed.
        STATE_ACTIVITY,          //!< Entity is active (processing).
        STATE_VISIBILITY,        //!< Entity is visible (if displayable).
        STATE_ACTIVE_SELECTION,  //!< Active entity of the selection.
        STATE_SELECTED           //!< Is selected entity
    };

    enum Role
    {
        ROLE_UNDEFINED = 0,
        ROLE_PROJECT,
        ROLE_FRAGMENT,
        ROLE_ASSET,
        ROLE_RESOURCE,
        ROLE_HUB,
        ROLE_STRUCTURAL_HUB
    };

    explicit Entity(const String &name, Entity *parent = nullptr);
    virtual ~Entity();

    void setParent(Entity *entity);

    Entity* parent();
    const Entity* parent() const;

    void setName(const String& name);
    const String& name() const;

    /**
     * @brief Set the entity unique object reference.
     */
    void setRef(const ObjectRef &ref);
    const ObjectRef& ref() const;

    /**
     * @brief Set the type reference of the instance of this entity.
     */
    void setTypeRef(const TypeRef &typeRef);
    const TypeRef& typeRef() const;

    /**
     * @brief Role of the entity.
     */
    virtual Role role() const = 0;

    /**
     * @brief Create step, related to destroy.
     */
    virtual void create() = 0;

    /**
     * @brief Destroy in correct order its children and then this.
     */
    virtual void destroy() = 0;

    /**
     * @brief Update content.
     */
    virtual void update() = 0;

    /**
     * @brief Returns true if the entity can be safely deleted.
     */
    virtual Bool deletable() const = 0;

    virtual Bool load() = 0;
    virtual Bool save() = 0;

    /**
     * @brief Project related to the entity.
     */
    virtual Project* project() = 0;

    /**
     * @brief Project related to the entity (const version).
     */
    virtual const Project* project() const = 0;

    /**
     * @brief Depending of the parent and child type, returns the index position
     * in the dedicated set of children.
     * @return -1 if not found.
     */
    virtual Int32 childIndexOf(Entity *entity) const = 0;

    /**
     * @brief By default it exists if the reference point to a project.
     */
    virtual Bool exists() const;

    /**
     * @brief Has changes to saved since last save(). Default ready the STATE_CHANGED flag.
     */
    virtual Bool hasChanges() const;

    /**
     * @brief Is the parent entity a hub.
     */
    virtual Bool isParentHub() const;

    /**
     * @brief Set (un)active entity of the selection.
     * @note Compaitble with selected.
     */
    virtual void setActiveSelection(Bool activity);

    /**
     * @brief Is the active entity of the selection.
     */
    Bool isActiveSelection() const;

    /**
     * @brief Set selected entity.
     * @note Compatible with active.
     */
    virtual void setSelected(Bool selected);

    /**
     * @brief Is entity part of the selection
     */
    Bool isSelected() const;

    /**
     * @brief Set activity of the entity (processing, update...)
     */
    virtual void setActivity(Bool activity);

    /**
     * @brief Is the entity active (processing, update...)
     */
    Bool isActive() const;

    /**
     * @brief Set visibility state of the entity (draw, picking...)
     */
    virtual void setVisibility(Bool visibility);

    /**
     * @brief Is the entity visibly (draw, picking...)
     */
    Bool isVisible() const;

    /**
     * @brief Serialize the entity content.
     */
    virtual Bool serializeContent(OutStream &stream) const;

    /**
     * @brief Deserialize the entity content.
     */
    virtual Bool deserializeContent(InStream &stream);

    Bool writeToFile(OutStream &os) const;
    Bool readFromFile(InStream &is);

protected:

    TypeRef m_typeRef;   //!< Related object reference type

    String m_name;       //!< Project display name
    ObjectRef m_ref;

    Entity *m_parent;

    BitSet64 m_capacities;  //!< Flags and capacities

    inline void setDirty() { m_capacities.enable(STATE_CHANGED); }
    inline void setClean() { m_capacities.disable(STATE_CHANGED); }
    inline Bool isDirty() const { return m_capacities.getBit(STATE_CHANGED); }
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ENTITY_H

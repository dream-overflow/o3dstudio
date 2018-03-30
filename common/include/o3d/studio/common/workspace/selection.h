/**
 * @brief Workspace current workspace current selection management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-08
 * @details
 */

#ifndef _O3DS_COMMON_SELECTION_H
#define _O3DS_COMMON_SELECTION_H

#include "workspace.h"
#include "selectionitem.h"

#include <set>

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace common {

class Hub;

/**
 * @brief The Selection manager.
 * @note Works on the active project of the current workspace.
 */
class O3S_API Selection : public BaseObject
{
public:

    enum AcceptRole
    {
        ACCEPT_ANY = 0,         //!< Any kind of entity.
        ACCEPT_PROJECT,         //!< Project only.
        ACCEPT_FRAGMENT,        //!< Fragment only.
        ACCEPT_HUB,             //!< Any kind of hub.
        ACCEPT_PROPERTY_HUB,    //!< Hub having properties to manage.
        ACCEPT_STRUCTURAL_HUB,  //!< Dummy hub, spacial hub...
        ACCEPT_RESOURCE         //!< Resource only.
    };

    Selection(BaseObject *parent = nullptr);
    virtual ~Selection();

    /**
     * @brief initialize after creation.
     */
    void initialize();

    /**
     * @brief Clear any selection.
     */
    void clear();

    /**
     * @brief Set accepted role/type.
     */
    void setAcceptedRole(AcceptRole accept);

    /**
     * @brief Current accepted role/type.
     */
    AcceptRole acceptedRole() const;

    /**
     * @brief begin a multiple selection that will be validated and thrown at endSelection call.
     */
    void beginSelection();

    void appendSelection(Entity *entity);

    /**
     * @brief endSelection Validate the selection transaction and emit signal.
     */
    void endSelection();

    /**
     * @brief select a single entity (project, fragment, hub) and emit signal.
     * @param project
     */
    void select(Entity *entity);

    /**
     * @brief unselect all current and emit signal.
     */
    void unselectAll();

    /**
     * @brief According to entity role and current selection profile test if the entity can be selected.
     */
    Bool canSelect(const Entity *entity) const;

    /**
     * @brief Set the current working/active entity.
     * Used to defined entity to works in some contexts.
     * @note By default the during a selection the active entity is defined to the first of the selection.
     */
    void setActiveEntity(Entity *entity);

    const LightRef& activeEntityRef() const;
    Entity* activeEntity();
    const Entity* activeEntity() const;

    const std::set<SelectionItem*> previousSelection() const;
    const std::set<SelectionItem*> currentSelection() const;

    const std::set<SelectionItem*> filterPrevious(const TypeRef &typeRef) const;
    const std::set<SelectionItem*> filterCurrent(const TypeRef &typeRef) const;

    const std::set<SelectionItem*> filterPreviousByBaseType(const TypeRef &typeRef) const;
    const std::set<SelectionItem*> filterCurrentByBaseType(const TypeRef &typeRef) const;

public /*signals*/:

    Signal<> selectionChanged{this};

public /*slots*/:

    void onChangeCurrentWorkspace(const String& name);
    void onProjectHubRemoved(LightRef ref);
    void onProjectFragmentRemoved(LightRef ref);
    void onProjectRemoved(LightRef ref);

private:

    Bool m_selecting;
    AcceptRole m_acceptRole;

    //! Current set of selected items
    std::set<SelectionItem*> m_currentSelection;
    //! Previous set of selected items
    std::set<SelectionItem*> m_previousSelection;

    //! Current set of selected items
    std::set<Entity*> m_selectingSet;

    //! Current active entity
    LightRef m_activeEntity;

    void cleanupAll();
    Bool checkEntityAcceptance(const Entity *entity) const;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTION_H

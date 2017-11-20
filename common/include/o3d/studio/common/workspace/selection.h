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

class O3S_API Selection : public BaseObject
{
public:

    Selection(BaseObject *parent = nullptr);
    virtual ~Selection();

    /**
     * @brief initialize with a different workspace
     * @param workspace
     */
    void initialize(Workspace *workspace);

    /**
     * @brief terminate Cleanup for current workspace
     */
    void terminate();

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

    const std::set<SelectionItem*> previousSelection() const;
    const std::set<SelectionItem*> currentSelection() const;

    const std::set<SelectionItem*> filterPrevious(const TypeRef &typeRef) const;
    const std::set<SelectionItem*> filterCurrent(const TypeRef &typeRef) const;

    const std::set<SelectionItem*> filterPreviousByBaseType(const TypeRef &typeRef) const;
    const std::set<SelectionItem*> filterCurrentByBaseType(const TypeRef &typeRef) const;

public /*signals*/:

    Signal<> selectionChanged{this};

private:

    Bool m_selecting;

    //! Related workspace
    Workspace *m_workspace;

    //! Current set of selected items
    std::set<SelectionItem*> m_currentSelection;
    //! Previous set of selected items
    std::set<SelectionItem*> m_previousSelection;

    //! Current set of selected items
    std::set<Entity*> m_selectingSet;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTION_H

/**
 * @brief Workspace current workspace current selection management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-08
 * @details
 */

#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"

using namespace o3d::studio::common;

Selection::Selection(BaseObject *parent) :
    BaseObject(parent),
    m_selecting(False),
    m_workspace(nullptr)
{

}

Selection::~Selection()
{
    if (m_workspace) {
        terminate();
    }
}

void Selection::initialize(Workspace *workspace)
{
    if (m_workspace) {
        terminate();
    }

    m_workspace = workspace;
}

void Selection::terminate()
{
    if (m_workspace) {
        for (SelectionItem *selectionItem : m_previousSelection) {
            delete selectionItem;
        }

        m_previousSelection.clear();

        for (SelectionItem *selectionItem : m_currentSelection) {
            delete selectionItem;
        }

        m_currentSelection.clear();

        m_selectingSet.clear();
        m_selecting = False;

        m_workspace = nullptr;
    }
}

void Selection::beginSelection()
{
    O3D_ASSERT(!m_selecting);

    if (m_selecting) {
        return;
    }

    m_selectingSet.clear();
}

void Selection::appendSelection(Entity *entity)
{
    if (!entity || !m_selecting) {
        return;
    }

    if (m_selectingSet.find(entity) != m_selectingSet.end()) {
        // already selected
        return;
    }

    m_selectingSet.insert(entity);
}

void Selection::endSelection()
{
    O3D_ASSERT(m_selecting);

    if (!m_selecting) {
        return;
    }

    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    for (Entity *entity : m_selectingSet) {
        m_currentSelection.insert(new SelectionItem(entity));
    }

    m_selectingSet.clear();
    m_selecting = False;

    selectionChanged();
}

void Selection::select(Entity *entity)
{
    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (entity) {
        m_currentSelection.insert(new SelectionItem(entity));
    }

    selectionChanged();
}

void Selection::unselectAll()
{
    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    selectionChanged();
}

const std::set<SelectionItem *> Selection::previousSelection() const
{
    return m_previousSelection;
}

const std::set<SelectionItem *> Selection::currentSelection() const
{
    return m_currentSelection;
}

const std::set<SelectionItem *> Selection::filterPrevious(const TypeRef &typeRef) const
{
    std::set<SelectionItem*> filtered;

    for (SelectionItem *selectionItem : m_previousSelection) {
        if (selectionItem->ref().typeId() == typeRef.id()) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

const std::set<SelectionItem *> Selection::filterCurrent(const TypeRef &typeRef) const
{
    std::set<SelectionItem*> filtered;

    for (SelectionItem *selectionItem : m_currentSelection) {
        if (selectionItem->ref().typeId() == typeRef.id()) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

const std::set<SelectionItem *> Selection::filterPreviousByBaseType(const TypeRef &typeRef) const
{
    std::set<SelectionItem*> filtered;

    for (SelectionItem *selectionItem : m_previousSelection) {
        if (selectionItem->ref().baseTypeOf(typeRef)) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

const std::set<SelectionItem *> Selection::filterCurrentByBaseType(const TypeRef &typeRef) const
{
    std::set<SelectionItem*> filtered;

    for (SelectionItem *selectionItem : m_currentSelection) {
        if (selectionItem->ref().baseTypeOf(typeRef)) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

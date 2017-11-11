/**
 * @brief Workspace current workspace current selection management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-08
 * @details
 */

#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"

using namespace o3d::studio::common;

Selection::Selection(QObject *parent) :
    QObject(parent),
    m_selecting(false),
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
        SelectionItem *selectionItem = nullptr;
        foreach (selectionItem, m_previousSelection) {
            delete selectionItem;
        }

        m_previousSelection.clear();

        selectionItem = nullptr;
        foreach (selectionItem, m_currentSelection) {
            delete selectionItem;
        }

        m_currentSelection.clear();

        m_selectingSet.clear();
        m_selecting = false;

        m_workspace = nullptr;
    }
}

void Selection::beginSelection()
{
    Q_ASSERT(!m_selecting);

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

    if (m_selectingSet.find(entity->ref().light()) != m_selectingSet.end()) {
        // already selected
        return;
    }

    m_selectingSet.insert(entity->ref().light());
}

void Selection::endSelection()
{
    Q_ASSERT(m_selecting);

    if (!m_selecting) {
        return;
    }

    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    LightRef ref;
    foreach (ref, m_selectingSet) {
        m_currentSelection.insert(new SelectionItem(ref));
    }

    m_selectingSet.clear();
    m_selecting = false;

    emit selectionChanged();
}

void Selection::select(Entity *entity)
{
    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (entity) {
        SelectionItem *selectionItem = new SelectionItem(entity->ref().light());
        m_currentSelection.insert(selectionItem);
    }

    emit selectionChanged();
}

void Selection::unselectAll()
{
    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    emit selectionChanged();
}

const QSet<SelectionItem *> Selection::previousSelection() const
{
    return m_previousSelection;
}

const QSet<SelectionItem *> Selection::currentSelection() const
{
    return m_currentSelection;
}

const QSet<SelectionItem *> Selection::filterPrevious(qint64 type) const
{
    QSet<SelectionItem*> filtered;

    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        if (selectionItem->ref().type() == type) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

const QSet<SelectionItem *> Selection::filterCurrent(qint64 type) const
{
    QSet<SelectionItem*> filtered;

    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_currentSelection) {
        if (selectionItem->ref().type() == type) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

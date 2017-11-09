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

        selectionItem = nullptr;
        foreach (selectionItem, m_selectingMap) {
            delete selectionItem;
        }

        m_selectingMap.clear();
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

    m_selectingMap.clear();
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

    foreach (selectionItem, m_selectingMap) {
        m_currentSelection.insert(selectionItem);
    }

    m_selectingMap.clear();

    emit selectionChanged();
}

void Selection::select(Project *project)
{
    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (project) {
        SelectionItem *selectionItem = new SelectionItem(SelectionItem::SELECTION_PROJECT, project->ref().light());
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

const QSet<SelectionItem *> Selection::filterPrevious(SelectionItem::SelectionType type) const
{
    QSet<SelectionItem*> filtered;

    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_previousSelection) {
        if (selectionItem->selectionType() == type) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

const QSet<SelectionItem *> Selection::filterCurrent(SelectionItem::SelectionType type) const
{
    QSet<SelectionItem*> filtered;

    SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, m_currentSelection) {
        if (selectionItem->selectionType() == type) {
            filtered.insert(selectionItem);
        }
    }

    return filtered;
}

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
#include "o3d/studio/common/workspace/fragment.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

#include <list>

using namespace o3d::studio::common;

Selection::Selection(BaseObject *parent) :
    BaseObject(parent),
    m_selecting(False),
    // m_acceptRole(ACCEPT_ANY)
    m_acceptRole(ACCEPT_STRUCTURAL_HUB)  // @todo
{

}

Selection::~Selection()
{
    // might be cleaned at workspace close signal
    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
        delete selectionItem;
    }

    if (m_activeEntity.isValid()) {
        // @todo do we unset an active flag ?
    }
}

void Selection::initialize()
{
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &Selection::onChangeCurrentWorkspace);

    // initial call manually
    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

void Selection::clear()
{
    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    m_previousSelection.clear();

    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
        delete selectionItem;
    }

    m_currentSelection.clear();

    if (m_activeEntity.isValid()) {
        // @todo do we unset an active flag ?
        m_activeEntity = LightRef();
    }

    m_selectingSet.clear();
    m_selecting = False;
}

void Selection::setAcceptedRole(Selection::AcceptRole accept)
{
    m_acceptRole = accept;
}

Selection::AcceptRole Selection::acceptedRole() const
{
    return m_acceptRole;
}

void Selection::beginSelection()
{
    O3D_ASSERT(!m_selecting);

    if (m_selecting) {
        return;
    }

    m_selecting = True;
    m_selectingSet.clear();
}

void Selection::appendSelection(Entity *entity)
{
    // is entity role currently accepted else reject mean ignore the selection attempt
    if (!checkEntityAcceptance(entity)) {
        return;
    }

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

    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    for (Entity *entity : m_selectingSet) {
        entity->setSelected(True);
        m_currentSelection.insert(new SelectionItem(entity));
    }

    if (m_activeEntity.isValid()) {
        // do we unset an active flag ?
    }

    if (m_selectingSet.begin() != m_selectingSet.end()) {
        // active entity is the first
        m_activeEntity = (*m_selectingSet.begin())->ref().light();
    } else {
        m_activeEntity = LightRef();
    }

    m_selectingSet.clear();
    m_selecting = False;

    selectionChanged();
}

void Selection::select(Entity *entity)
{
    // is entity role currently accepted else reject mean ignore the selection attempt
    if (!checkEntityAcceptance(entity)) {
        return;
    }

    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (m_activeEntity.isValid()) {
        // do we unset an active flag ?
    }

    if (entity) {
        entity->setSelected(True);
        m_currentSelection.insert(new SelectionItem(entity));

        // active entity is the unique
        m_activeEntity = entity->ref().light();
    } else {
        m_activeEntity = LightRef();
    }

    selectionChanged();
}

void Selection::unselectAll()
{
    // nothing to do if current selection is already empty
    if (m_currentSelection.empty()) {
        return;
    }

    for (SelectionItem *selectionItem : m_previousSelection) {
        delete selectionItem;
    }

    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (m_activeEntity.isValid()) {
        // do we unset an active flag ?
    }

    m_activeEntity = LightRef();

    selectionChanged();
}

void Selection::setActiveEntity(Entity *entity)
{
    if (entity) {
        m_activeEntity = entity->ref().light();
    } else {
        m_activeEntity = LightRef();
    }
}

const LightRef &Selection::activeEntityRef() const
{
    return m_activeEntity;
}

Entity *Selection::activeEntity()
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();
    return workspace->entity(m_activeEntity);
}

const Entity *Selection::activeEntity() const
{
    const common::Workspace *workspace = common::Application::instance()->workspaces().current();
    return workspace->entity(m_activeEntity);
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

void Selection::onChangeCurrentWorkspace(const o3d::String &name)
{
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    common::Workspace *workspace = workspaceManager->current();

    if (workspace) {
        workspace->onProjectRemoved.connect(this, &Selection::onProjectRemoved);
        workspace->onProjectHubRemoved.connect(this, &Selection::onProjectHubRemoved);
        workspace->onProjectFragmentRemoved.connect(this, &Selection::onProjectFragmentRemoved);
    }

    Bool lemit = False;

    if (!m_currentSelection.empty()) {
        lemit = True;
    }

    // cleanup all
    cleanupAll();

    if (lemit) {
        selectionChanged();
    }
}

void Selection::onProjectHubRemoved(LightRef ref)
{
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    common::Workspace *workspace = workspaceManager->current();

    if (!workspace) {
        return;
    }

    Project *project = workspace->activeProject();
    if (!project) {
        return;
    }

    Entity *entity = project->lookup(ref);
    if (!entity) {
        return;
    }

    entity->setSelected(False);

    if (m_activeEntity == ref) {
        // do we unset an active flag ?
        m_activeEntity = LightRef();
    }

    if (entity->ref().light().baseTypeOf(TypeRef::hub())) {
        const Hub *removedHub = static_cast<const Hub*>(entity);
        std::list<const Hub*> hubs = removedHub->hubs(True);

        std::list<std::set<SelectionItem*>::iterator> removeList;

        // erase hubs selection from current selection
        for (auto it1 = m_currentSelection.begin(); it1 != m_currentSelection.end(); ++it1) {
            for (auto cit2 = hubs.cbegin(); cit2 != hubs.cend(); ++cit2) {
                if ((*it1)->ref() == (*cit2)->ref().light()) {
                    removeList.push_back(it1);
                }
            }
        }

        // remove from current and set into previous selection
        if (!removeList.empty()) {
            m_previousSelection.clear();

            for (auto it = removeList.begin(); it != removeList.end(); ++it) {
                m_currentSelection.erase(*it);
                m_previousSelection.insert(**it);
            }

            selectionChanged();
        }
    }
}

void Selection::onProjectFragmentRemoved(LightRef ref)
{
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    common::Workspace *workspace = workspaceManager->current();

    if (!workspace) {
        return;
    }

    Project *project = workspace->activeProject();
    if (!project) {
        return;
    }

    Entity *entity = project->lookup(ref);
    if (!entity) {
        return;
    }

    entity->setSelected(False);

    if (m_activeEntity == ref) {
        // do we unset an active flag ?
        m_activeEntity = LightRef();
    }

    if (entity->ref().light().baseTypeOf(TypeRef::fragment())) {
        // erase selection from current selection
        for (auto it1 = m_currentSelection.begin(); it1 != m_currentSelection.end(); ++it1) {
            if ((*it1)->ref() == ref) {
                m_currentSelection.erase(it1);

                m_previousSelection.clear();
                m_previousSelection.insert(*it1);

                selectionChanged();
                return;
            }
        }
    }
}

void Selection::onProjectRemoved(LightRef ref)
{
    Bool lemit = False;

    if (!m_currentSelection.empty()) {
        lemit = True;
    }

    // cleanup all
    cleanupAll();

    if (m_activeEntity == ref) {
        // do we unset an active flag ?
        m_activeEntity = LightRef();
    }

    if (lemit) {
        selectionChanged();
    }
}

void Selection::cleanupAll()
{
    for (SelectionItem *selectionItem : m_currentSelection) {
        selectionItem->unselect();
    }

    m_previousSelection = m_currentSelection;
    m_currentSelection.clear();

    if (m_activeEntity.isValid()) {
        // do we unset an active flag ?
    }

    m_activeEntity = LightRef();
}

o3d::Bool Selection::checkEntityAcceptance(Entity *entity)
{
    // is entity role currently accepted else reject mean ignore the selection attempt
    if (entity) {
        if (m_acceptRole == ACCEPT_STRUCTURAL_HUB) {
            if (entity->role() != Entity::ROLE_STRUCTURAL_HUB) {
                return False;
            }
        }
        // @todo
    }

    return True;
}

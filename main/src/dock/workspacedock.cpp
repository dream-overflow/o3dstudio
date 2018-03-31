/**
 * @brief Application main workspace dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#include "workspacedock.h"
#include "../mainwindow.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/command/hub/removehubcommand.h"
#include "o3d/studio/common/command/fragment/removefragmentcommand.h"

#include <QtWidgets/QTreeView>

using namespace o3d::studio::main;


WorkspaceDock::WorkspaceDock(BaseObject *parent) :
    BaseObject(parent),
    common::Dock(),
    m_lastSelected(nullptr),
    m_autoExpand(True)
{
    setupUi();

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &WorkspaceDock::onChangeCurrentWorkspace);

     // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &WorkspaceDock::onSelectManagerChange);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

WorkspaceDock::~WorkspaceDock()
{
    // managed deletion
    // deletePtr(m_qtWorkspaceDock);
}

void WorkspaceDock::setupUi()
{
    m_qtWorkspaceDock = new QtWorkspaceDock(nullptr);
}

QDockWidget *WorkspaceDock::ui()
{
    return m_qtWorkspaceDock;
}

o3d::String WorkspaceDock::elementName() const
{
    return "o3s::main::workspace";
}

Qt::DockWidgetArea WorkspaceDock::dockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}

void WorkspaceDock::setAutoExpand(o3d::Bool s)
{
    m_autoExpand = s;
}

o3d::Bool WorkspaceDock::isAutoExpand() const
{
    return m_autoExpand;
}

void WorkspaceDock::onAddProject(LightRef ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());
    common::Project *project = common::Application::instance()->workspaces().current()->project(ref);

    // add a new project item
    if (project) {
        common::ProjectItem *item = projectModel->addProject(project);
        QModelIndex index = projectModel->index(item->row(), 0, QModelIndex());
        m_qtWorkspaceDock->m_treeView->expand(index);
    }
}

void WorkspaceDock::onRemoveProject(LightRef ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());

    // remove a project item
    projectModel->removeProject(ref);
}

void WorkspaceDock::onActivateProject(LightRef ref)
{
    common::UiController &uiCtrl = common::Application::instance()->ui();

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::Project *project = workspace->project(ref);

    if (!project) {
        // set to default view
        common::Content *content = uiCtrl.content("o3s::main::browsercontent");
        if (content) {
            uiCtrl.setActiveContent(content, true);
        }
        return;
    }

    common::O3DCanvasContent *content = static_cast<common::O3DCanvasContent*>(
        uiCtrl.content("o3s::main::o3dcanvascontent::" + project->ref().light().longId()));

    if (content) {
        uiCtrl.setActiveContent(content, true);
    }
}

void WorkspaceDock::onProjectHubAdded(LightRef ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());
    common::Hub *hub = workspace->findHub(ref);

    // add a new project hub
    if (hub) {
        projectModel->addHub(hub);
    }
}

void WorkspaceDock::onProjectHubRemoved(LightRef ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());

    // remove a project hub
    projectModel->removeHub(ref);
}

void WorkspaceDock::onProjectFragmentAdded(LightRef ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());
    common::Fragment *fragment = workspace->fragment(ref);

    // add a new project fragment
    if (fragment) {
        projectModel->addFragment(fragment);
    }
}

void WorkspaceDock::onProjectFragmentRemoved(LightRef ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());

    // remove a project fragment
    projectModel->removeFragment(ref);
}

void WorkspaceDock::onProjectEntityChanged(o3d::studio::common::LightRef ref, o3d::BitSet64 changes)
{
    if (changes.getBit(common::Entity::PRESENTATION_CHANGED)) {
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        if (!workspace) {
            return;
        }

        common::Project* project = workspace->project(ref);
        if (!project) {
            return;
        }

        common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());
        projectModel->updatePresentation(ref);
    }
}

void WorkspaceDock::onSelectManagerChange()
{
    const std::set<common::SelectionItem *> previousSelection = common::Application::instance()->selection().previousSelection();
    const std::set<common::SelectionItem *> currentSelection = common::Application::instance()->selection().currentSelection();

    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtWorkspaceDock->m_treeView->model());
    common::ProjectItem *projectItem = nullptr;

    QModelIndex current;

    for (common::SelectionItem *selectionItem : previousSelection) {
        projectItem = projectModel->find(selectionItem->ref());

        if (!projectItem) {
            continue;
        }

        current = projectModel->modelIndex(projectItem);
        projectItem->setSelected(False);

        projectModel->updatePresentation(current);
    }

    for (common::SelectionItem *selectionItem : currentSelection) {
        projectItem = projectModel->find(selectionItem->ref());

        if (!projectItem) {
            continue;
        }

        current = projectModel->modelIndex(projectItem);
        projectItem->setSelected(True);

        projectModel->updatePresentation(current);

        // auto expand on select
        if (m_autoExpand) {
            current = current.parent();
            while (current.isValid()) {
                if (!m_qtWorkspaceDock->m_treeView->isExpanded(current)) {
                    m_qtWorkspaceDock->m_treeView->expand(current);
                }

                current = current.parent();
            }
        }
    }
}

void WorkspaceDock::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::ProjectModel *model = new common::ProjectModel();
    m_qtWorkspaceDock->setModel(model);

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // workspace project
        workspace->onProjectAdded.connect(this, &WorkspaceDock::onAddProject);
        workspace->onProjectRemoved.connect(this, &WorkspaceDock::onRemoveProject);
        workspace->onProjectActivated.connect(this, &WorkspaceDock::onActivateProject);

        // project entities
        workspace->onProjectHubAdded.connect(this, &WorkspaceDock::onProjectHubAdded, CONNECTION_ASYNCH);
        workspace->onProjectHubRemoved.connect(this, &WorkspaceDock::onProjectHubRemoved, CONNECTION_ASYNCH);

        workspace->onProjectFragmentAdded.connect(this, &WorkspaceDock::onProjectFragmentAdded, CONNECTION_ASYNCH);
        workspace->onProjectFragmentRemoved.connect(this, &WorkspaceDock::onProjectFragmentRemoved, CONNECTION_ASYNCH);

        workspace->onProjectEntityChanged.connect(this, &WorkspaceDock::onProjectEntityChanged, CONNECTION_ASYNCH);
    }
}

QtWorkspaceDock::QtWorkspaceDock(QWidget *parent) :
    QDockWidget(tr("Workspace"), parent),
    m_lastSelected(nullptr)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
    setFocusPolicy(Qt::StrongFocus);

    // details
    connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onSelectionDetails(const QModelIndex &)));
    connect(m_treeView, SIGNAL(pressed(const QModelIndex &)), SLOT(onPressItem(const QModelIndex &)));
}

QtWorkspaceDock::~QtWorkspaceDock()
{
    delete m_treeView;
}

void QtWorkspaceDock::onSelectionDetails(const QModelIndex &)
{
    QModelIndex current = m_treeView->currentIndex();

    if (current.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        common::Project *project = projectItem->project();
        if (project) {
            // @todo
        }
    }
}

void QtWorkspaceDock::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
/*    if (!(QApplication::mouseButtons() & Qt::RightButton) && !(QApplication::mouseButtons() & Qt::MiddleButton)) {
        return;
    }

    if (previous.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(previous.internalPointer());
        m_lastSelected = nullptr;
    }

    if (current.isValid()) {
        Bool multiple = False;

        if (m_shiftModifier) {
            multiple = True;
        }

        common::Entity *selected = nullptr;

        // @todo for multiple selection
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        m_lastSelected = projectItem;

        common::TypeRef baseType = common::Application::instance()->types().baseTypeRef(projectItem->ref().typeId());

        // first set as active projet if not current
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        common::Project *project = workspace->project(projectItem->ref());

        common::Selection &selection = common::Application::instance()->selection();

        if (!project) {
            return;
        }

        if (workspace->activeProject() != project) {
            workspace->setActiveProject(project->ref().light());
        }

        if (projectItem->ref().baseTypeOf(common::TypeRef::hub())) {
            common::Hub *hub = project->rootHub()->findHub(projectItem->ref().id());
            if (hub) {
                // hub select but according to accepted role :
                // @todo for now auto defines accepted role by hub
                if (hub->role() == common::Entity::ROLE_STRUCTURAL_HUB) {
                    selection.setAcceptedRole(common::Selection::ACCEPT_STRUCTURAL_HUB);
                } else {
                    selection.setAcceptedRole(common::Selection::ACCEPT_HUB);
                    // selection.setAcceptedRole(common::Selection::ACCEPT_PROPERTY_HUB);
                }

                if (selection.acceptedRole() == common::Selection::ACCEPT_PROJECT) {
                    selected = hub->project();
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_STRUCTURAL_HUB) {
                    common::Hub *currentHub = hub;

                    // select the first found structure parent hub
                    while (currentHub->role() != common::Entity::ROLE_STRUCTURAL_HUB) {
                        if (currentHub->isParentHub()) {
                            currentHub = static_cast<common::Hub*>(currentHub->parent());
                        }
                    }

                    if (currentHub->role() == common::Entity::ROLE_STRUCTURAL_HUB) {
                        selected == currentHub;
                    }
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_HUB) {
                    // no specificity
                    selected = hub;
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_PROPERTY_HUB) {
                    // @todo property hub ?
                    selected = hub;
                } else {
                    selected = hub->project();
                }
            }
        } else if (projectItem->ref().baseTypeOf(common::TypeRef::fragment())) {
            common::Fragment *fragment = project->fragment(projectItem->ref());
            if (fragment) {
                // fragment select but according to accepted role :
                // @todo
                if (selection.acceptedRole() == common::Selection::ACCEPT_PROJECT) {
                    selected = fragment->project();
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_STRUCTURAL_HUB) {
                    selected = fragment->project()->rootHub();
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_HUB) {
                    selected = fragment->project()->rootHub();
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_PROPERTY_HUB) {
                    selected = project->rootHub();
                } else if (selection.acceptedRole() == common::Selection::ACCEPT_FRAGMENT) {
                    selected = fragment;
                } else {
                    selected = fragment->project();
                }
            }
        } else if (projectItem->ref().baseTypeOf(common::TypeRef::project())) {
            // project select but according to accepted role :
            if (selection.acceptedRole() == common::Selection::ACCEPT_PROJECT) {
                selected = project;
            } else if (selection.acceptedRole() == common::Selection::ACCEPT_STRUCTURAL_HUB) {
                selected = project->rootHub();
            } else if (selection.acceptedRole() == common::Selection::ACCEPT_HUB) {
                selected = project->rootHub();
            } else if (selection.acceptedRole() == common::Selection::ACCEPT_PROPERTY_HUB) {
                selected = project->rootHub();
            } else {
                selected = project;
            }
        } else if (projectItem->ref().baseTypeOf(common::TypeRef::resource())) {
            // @todo when resources will be implemented
        }

        if (selected) {
            if (multiple) {
                common::Entity* entity;

                auto previous = selection.filterCurrentByBaseType(common::TypeRef::hub());

                selection.beginSelection();

                for (common::SelectionItem *item : previous) {
                    entity = item->entity();

                    if ((item->ref() != selected->ref().light()) && (entity = findSelectable(entity))) {
                        selection.appendSelection(entity);
                    }
                }

                if (!selected->isSelected() && (entity = findSelectable(selected))) {
                    // if not selected add it to last
                    selection.appendSelection(entity);
                }

                selection.endSelection();
            } else {
                if (!selected->isSelected()) {
                    selection.select(selected);
                } else {
                    selection.unselectAll();
                }
            }
        }
    }*/
}

o3d::studio::common::Entity* QtWorkspaceDock::findSelectable(common::Entity *entity) const
{
    if (!entity) {
        return nullptr;
    }

    if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_ANY) {
        return entity;
    }

    if (entity->ref().light().baseTypeOf(common::TypeRef::project())) {
        if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_PROJECT) {
            return entity;
        }
    } else if (entity && entity->ref().light().baseTypeOf(common::TypeRef::fragment())) {
        if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_FRAGMENT) {
            return entity;
        }
    } else if (entity && entity->ref().light().baseTypeOf(common::TypeRef::resource())) {
        if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_RESOURCE) {
            return entity;
        }
    } else if (entity && entity->ref().light().baseTypeOf(common::TypeRef::hub())) {
        if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_STRUCTURAL_HUB) {
            if (entity->role() == common::Entity::ROLE_STRUCTURAL_HUB) {
                return entity;
            }
        } else if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_PROPERTY_HUB) {
            if (entity->role() == common::Entity::ROLE_HUB) {
                return entity;
            }
        } else if (common::Application::instance()->selection().acceptedRole() == common::Selection::ACCEPT_HUB) {
            if (entity->role() == common::Entity::ROLE_HUB) {
                return entity;
            }
        }
    }

    return nullptr;
}

void QtWorkspaceDock::onPressItem(const QModelIndex &current)
{
    if (current.isValid()) {
        Bool multiple = False;

        if (QApplication::mouseButtons() & Qt::RightButton) {
            if (!m_treeView->selectionModel()->isSelected(current)) {
                // single of view-selection
                m_treeView->selectionModel()->select(current, QItemSelectionModel::Clear | QItemSelectionModel::Select);
            }
        }

        if (!(QApplication::mouseButtons() & Qt::MiddleButton)) {
            // select with middle-click or maybee @todo when left click the icone
            return;
        }

        if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
            multiple = True;
        }

        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        m_lastSelected = projectItem;

        // common::TypeRef baseType = common::Application::instance()->types().baseTypeRef(projectItem->ref().typeId());

        // first set as active projet if not current
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        common::Project *project = workspace->project(projectItem->ref());

        common::Selection &selection = common::Application::instance()->selection();

        if (!project) {
            return;
        }

        if (workspace->activeProject() != project) {
            workspace->setActiveProject(project->ref().light());
        }

        common::Entity *currentEntity = project->lookup(projectItem->ref());

        if (currentEntity && (selection.currentSelection().empty() || !multiple)) {
            // set new current accept role on initial selection
            if (currentEntity->role() == common::Entity::ROLE_STRUCTURAL_HUB) {
                selection.setAcceptedRole(common::Selection::ACCEPT_STRUCTURAL_HUB);
            } else if (currentEntity->role() == common::Entity::ROLE_HUB) {
                selection.setAcceptedRole(common::Selection::ACCEPT_HUB);
            } else if (currentEntity->role() == common::Entity::ROLE_PROJECT) {
                selection.setAcceptedRole(common::Selection::ACCEPT_PROJECT);
            } else if (currentEntity->role() == common::Entity::ROLE_FRAGMENT) {
                selection.setAcceptedRole(common::Selection::ACCEPT_FRAGMENT);
            } else if (currentEntity->role() == common::Entity::ROLE_RESOURCE) {
                selection.setAcceptedRole(common::Selection::ACCEPT_RESOURCE);
            }
        }

        if (selection.canSelect(currentEntity)) {
            if (multiple) {
                // toggle select in multiple
                common::Entity* entity;

                auto previous = selection.currentSelection();

                selection.beginSelection();

                for (common::SelectionItem *item : previous) {
                    entity = item->entity();

                    if ((item->ref() != currentEntity->ref().light())) {
                        selection.appendSelection(entity);
                    }
                }

                if (!currentEntity->isSelected()) {
                    // if not selected add it to last
                    selection.appendSelection(currentEntity);
                }

                selection.endSelection();
            } else {
                if (!currentEntity->isSelected()) {
                    selection.select(currentEntity);
                } else {
                    selection.unselectAll();
                }
            }
        } else {
            if (!multiple) {
                selection.select(currentEntity);
            }
        }
    }

    // onSelectionChanged(index, index);
}

void QtWorkspaceDock::focusInEvent(QFocusEvent *event)
{
    QDockWidget::focusInEvent(event);
}

void QtWorkspaceDock::focusOutEvent(QFocusEvent *event)
{
    QDockWidget::focusOutEvent(event);
}

void QtWorkspaceDock::keyPressEvent(QKeyEvent *event)
{
    QModelIndex currentIndex = m_treeView->currentIndex();

    if (m_treeView->hasFocus() && currentIndex.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(currentIndex.internalPointer());

        if (event->key() == Qt::Key_Delete) {
            // cannot command the delete the root hub
            if (projectItem->isHub() && !projectItem->parentItem()->isProject()) {
                common::RemoveHubCommand *cmd = new common::RemoveHubCommand(projectItem->ref(), projectItem->parentItem()->ref());
                common::Application::instance()->command().addCommand(cmd);
            } else if (projectItem->isFragment()) {
                common::RemoveFragmentCommand *cmd = new common::RemoveFragmentCommand(projectItem->ref(), projectItem->parentItem()->ref());
                common::Application::instance()->command().addCommand(cmd);
            }
        }
    }

    return QDockWidget::keyPressEvent(event);
}

void QtWorkspaceDock::keyReleaseEvent(QKeyEvent *event)
{
    return QDockWidget::keyReleaseEvent(event);
}

bool QtWorkspaceDock::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_treeView && event->type() == QEvent::MouseButtonPress) {
        QContextMenuEvent *m = static_cast<QContextMenuEvent*>(event);
        if (event->type() == QEvent::ContextMenu) {
            // @todo create context menu
            return true;
        }
    }
    return false;
}

void QtWorkspaceDock::setModel(QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = m_treeView->model();
    m_treeView->setModel(model);

    deletePtr(oldModel);

    // selection
    QItemSelectionModel *selectionModel = m_treeView->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSelectionChanged(const QModelIndex &, const QModelIndex &)));
}

void QtWorkspaceDock::setupUi()
{
    setWindowIcon(QIcon(":/icons/work_black.svg"));

    m_treeView = new QTreeView(this);
    setWidget(m_treeView);

    m_treeView->setHeaderHidden(true);
    m_treeView->setSelectionMode(QTreeView::MultiSelection);
    setModel(new common::ProjectModel());

    // install a custom context menu
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->installEventFilter(this);

    connect(m_treeView, &QTreeView::customContextMenuRequested, this, &QtWorkspaceDock::onShowContextMenu);
}

void QtWorkspaceDock::onShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = m_treeView->mapToGlobal(pos);
    QMenu contextMenu;

    QModelIndex index = m_treeView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QModelIndex current = m_treeView->currentIndex();
    // @todo select model selection

    QModelIndexList indexes = m_treeView->selectionModel()->selectedIndexes();

    if (indexes.empty()) {
        return;
    }

    Bool single = False;
    if (indexes.size() == 1) {
        single = True;

        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        common::Project *project = projectItem->project();
        if (project) {
            // @todo
        }
    }

    QAction *selectAction = new QAction(tr("Select"));
    selectAction->setProperty("actionName", "select");
    contextMenu.addAction(selectAction);

    QAction *unselectAction = new QAction(tr("Unselect"));
    unselectAction->setProperty("actionName", "unselect");
    contextMenu.addAction(unselectAction);

    contextMenu.addSeparator();

    if (single && (index.flags() & Qt::ItemIsEditable)) {
        QAction *renameAction = new QAction(tr("Rename"));
        renameAction->setProperty("actionName", "rename");
        contextMenu.addAction(renameAction);
        contextMenu.addSeparator();
    }

    QAction *deleteAction = new QAction(tr("Delete"));
    deleteAction->setProperty("actionName", "delete");
    contextMenu.addAction(deleteAction);

    contextMenu.addSeparator();

    QAction *cancelAction = new QAction(tr("Cancel"));
    cancelAction->setProperty("actionName", "cancel");
    contextMenu.addAction(cancelAction);

    QAction* selectedItem = contextMenu.exec(globalPos);
    if (selectedItem) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(index.internalPointer());

        if (selectedItem->property("actionName") == "rename") {
            m_treeView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
            m_treeView->edit(index);

        } else if (selectedItem->property("actionName") == "select") {

        } else if (selectedItem->property("actionName") == "unselect") {

        } else if (selectedItem->property("actionName") == "delete") {

        }
    }
}

/**
 * @brief Application main selection dock
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-18
 * @details
 */

#include "selectiondock.h"
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

#include <QtWidgets/QTreeView>

using namespace o3d::studio::main;

SelectionDock::SelectionDock(BaseObject *parent) :
    BaseObject(parent),
    common::Dock(),
    m_qtSelectionDock(nullptr),
    m_lastSelected(nullptr),
    m_autoFollow(True)
{
    setupUi();

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &SelectionDock::onChangeCurrentWorkspace);

     // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &SelectionDock::onSelectionChanged);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

SelectionDock::~SelectionDock()
{
    // managed deletion
    // deletePtr(m_qtSelectionDock);
}

void SelectionDock::setupUi()
{
    m_qtSelectionDock = new QtSelectionDock(nullptr);
}

QDockWidget *SelectionDock::ui()
{
    return m_qtSelectionDock;
}

o3d::String SelectionDock::elementName() const
{
    return "o3s::main::selection";
}

Qt::DockWidgetArea SelectionDock::dockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}

void SelectionDock::setAutoFollow(o3d::Bool s)
{
    m_autoFollow = s;
}

o3d::Bool SelectionDock::isAutoFollow() const
{
    return m_autoFollow;
}

//void SelectionDock::onActivateProject(LightRef ref)
//{
//    common::UiController &uiCtrl = common::Application::instance()->ui();

//    common::Workspace* workspace = common::Application::instance()->workspaces().current();
//    common::Project *project = workspace->project(ref);

//    if (!project) {
//        // set to default view
//        common::Content *content = uiCtrl.content("o3s::main::browsercontent");
//        if (content) {
//            uiCtrl.setActiveContent(content, true);
//        }
//        return;
//    }

//    common::O3DCanvasContent *content = static_cast<common::O3DCanvasContent*>(
//        uiCtrl.content("o3s::main::o3dcanvascontent::" + project->ref().light().longId()));

//    if (content) {
//        uiCtrl.setActiveContent(content, true);
//    }
//}

void SelectionDock::onSelectionChanged()
{
    const std::set<common::SelectionItem *> previousSelection = common::Application::instance()->selection().previousSelection();
    const std::set<common::SelectionItem *> currentSelection = common::Application::instance()->selection().currentSelection();
/*
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_qtSelectionDock->m_listView->model());
    common::ProjectItem *projectItem = nullptr;

    QModelIndex current;

    common::SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, previousSelection) {
        projectItem = projectModel->find(selectionItem->ref());

        if (!projectItem) {
            continue;
        }

        current = projectModel->modelIndex(projectItem);
        m_qtSelectionDock->m_treeView->selectionModel()->select(current, QItemSelectionModel::Deselect);
    }

    selectionItem = nullptr;
    foreach (selectionItem, currentSelection) {
        projectItem = projectModel->find(selectionItem->ref());

        if (!projectItem) {
            continue;
        }

        current = projectModel->modelIndex(projectItem);
        m_qtSelectionDock->m_treeView->selectionModel()->select(current, QItemSelectionModel::Select);

        // auto expand on select
        if (m_autoExpand) {
            current = current.parent();
            while (current.isValid()) {
                if (!m_qtSelectionDock->m_treeView->isExpanded(current)) {
                    m_qtSelectionDock->m_treeView->expand(current);
                }

                current = current.parent();
            }
        }
    }*/
}

void SelectionDock::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::ProjectModel *model = new common::ProjectModel();
    m_qtSelectionDock->setModel(model);
/*
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
    }*/
}

QtSelectionDock::QtSelectionDock(QWidget *parent) :
    QDockWidget(tr("Selection"), parent),
    m_lastSelected(nullptr)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
    setFocusPolicy(Qt::StrongFocus);

    // details
 //   connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onSelectionDetails(const QModelIndex &)));
 //   connect(m_treeView, SIGNAL(clicked(const QModelIndex &)), SLOT(onSelectItem(const QModelIndex &)));
}

QtSelectionDock::~QtSelectionDock()
{
    delete m_listView;
}
/*
void QtSelectionDock::onSelectionDetails(const QModelIndex &)
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

void QtSelectionDock::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) {
        // @todo for multiple selection
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(previous.internalPointer());
        m_lastSelected = nullptr;

        // projectItem->unselect();
    }

    if (current.isValid()) {
        // @todo for multiple selection
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        m_lastSelected = projectItem;

        common::TypeRef baseType = common::Application::instance()->types().baseTypeRef(projectItem->ref().typeId());

        // first set as active projet if not current
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        common::Project *project = workspace->project(projectItem->ref());

        if (!project) {
            return;
        }

        if (workspace->activeProject() != project) {
            workspace->setActiveProject(project->ref().light());
        }

        if (projectItem->ref().baseTypeOf(common::TypeRef::hub())) {
            common::Hub *hub = project->rootHub()->findHub(projectItem->ref().id());
            if (hub) {
                common::Application::instance()->selection().select(hub);
            }
        } else if (projectItem->ref().baseTypeOf(common::TypeRef::fragment())) {
            common::Fragment *fragment = project->fragment(projectItem->ref());
            if (fragment) {
                common::Application::instance()->selection().select(fragment);
            }
        } else if (projectItem->ref().baseTypeOf(common::TypeRef::project())) {
            common::Application::instance()->selection().select(project);
        }
    }
}
*/
void QtSelectionDock::onSelectItem(const QModelIndex &/*index*/)
{
    // @todo should be unset when selection change
    // onSelectionChanged(index, index);
    return;
/*
    // on click second time, to reselect
    // @todo should only arrives after a lost of focus or a change of selection...
    if (index.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(index.internalPointer());
        if (m_lastSelected != nullptr && m_lastSelected == projectItem) {
            common::Project *project = projectItem->project();
            common::TypeRef baseType = common::Application::instance()->types().baseTypeRef(projectItem->ref().type());

            if (!project) {
                return;
            }

            // first set as active projet if not current
            common::Workspace* workspace = common::Application::instance()->workspaces().current();
            if (workspace->activeProject() != project) {
                workspace->setActiveProject(project->ref().light());
            }

            if (baseType == common::TypeRef::hub()) {
                common::Hub *hub = project->rootHub()->findHub(projectItem->ref().id());
                if (hub) {
                    common::Application::instance()->selection().select(hub);
                }
            } else if (baseType == common::TypeRef::fragment()) {
                common::Fragment *fragment = project->fragment(projectItem->ref());
                if (fragment) {
                    common::Application::instance()->selection().select(fragment);
                }
            } else if (baseType == common::TypeRef::project()) {
                common::Application::instance()->selection().select(project);
            }
        }
    }*/
}


void QtSelectionDock::focusInEvent(QFocusEvent *event)
{
    QDockWidget::focusInEvent(event);
}

void QtSelectionDock::keyPressEvent(QKeyEvent *event)
{
 /*   QModelIndex currentIndex = m_treeView->currentIndex();

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
*/
    return QDockWidget::keyPressEvent(event);
}

void QtSelectionDock::setModel(QAbstractItemModel *model)
{
  /*  QAbstractItemModel *oldModel = m_treeView->model();
    m_treeView->setModel(model);

    deletePtr(oldModel);

    // selection
    QItemSelectionModel *selectionModel = m_treeView->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSelectionChanged(const QModelIndex &, const QModelIndex &)));
*/}

void QtSelectionDock::setupUi()
{
    setWindowIcon(QIcon(":/icons/center_focus_weak_black.svg"));

    m_listView = new QListView(this);
    setWidget(m_listView);

    // m_treeView->setHeaderHidden(true);
    // setModel(new common::ProjectModel());
}

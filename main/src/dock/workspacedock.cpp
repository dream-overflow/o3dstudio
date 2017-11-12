/**
 * @brief Application main workspace dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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
#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include <QtWidgets/QTreeView>

using namespace o3d::studio::main;


WorkspaceDock::WorkspaceDock(QWidget *parent) :
    QDockWidget(tr("Workspace"), parent),
    common::Dock(),
    m_lastSelected(nullptr)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
    setFocusPolicy(Qt::StrongFocus);

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    connect(workspaceManager, SIGNAL(onWorkspaceActivated(QString)), SLOT(onChangeCurrentWorkspace(QString)));

    onChangeCurrentWorkspace(workspaceManager->current()->name());

    // selection
    QItemSelectionModel *selectionModel = m_treeView->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSelectionChanged(const QModelIndex &, const QModelIndex &)));

    // details
    connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onSelectionDetails(const QModelIndex &)));
    connect(m_treeView, SIGNAL(clicked(const QModelIndex &)), SLOT(onSelectItem(const QModelIndex &)));

    // selection manager
    connect(&common::Application::instance()->selection(), SIGNAL(selectionChanged()), SLOT(onSelectManagerChange()));
}

WorkspaceDock::~WorkspaceDock()
{
    delete m_treeView;
}

QDockWidget *WorkspaceDock::ui()
{
    return this;
}

QString WorkspaceDock::elementName() const
{
    return("o3s::main::workspace");
}

Qt::DockWidgetArea WorkspaceDock::dockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}

void WorkspaceDock::onAddProject(const common::LightRef &ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Project *project = common::Application::instance()->workspaces().current()->project(ref);

    // add a new project item
    if (project) {
        projectModel->addProject(project);
    }
}

void WorkspaceDock::onRemoveProject(const common::LightRef &ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Project *project = common::Application::instance()->workspaces().current()->project(ref);

    // remove a project item
    projectModel->removeProject(ref);
}

void WorkspaceDock::onActivateProject(const common::LightRef &ref)
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
        uiCtrl.content(QString("o3s::main::o3dcanvascontent::") + project->ref().light().longId()));

    if (content) {
        uiCtrl.setActiveContent(content, true);
    }
}

void WorkspaceDock::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(previous.internalPointer());
        m_lastSelected = nullptr;

        // projectItem->unselect();
    }

    if (current.isValid()) {
        // @todo for multiple selection only
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        m_lastSelected = projectItem;

        // projectItem->select();

        /*common::Project *project = projectItem->project();
        if (project) {
            common::Application::instance()->selection().select(project);
            // project->workspace()->selectProject(project->ref());
        }*/
    }
}

void WorkspaceDock::onSelectionDetails(const QModelIndex &)
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

void WorkspaceDock::onSelectItem(const QModelIndex &index)
{
    // @todo should be unset when selection change
    // onSelectionChanged(index, index);

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
                common::Hub *hub = project->findHub(projectItem->ref().id());
                if (hub) {
                    common::Application::instance()->selection().select(hub);
                }
            } else if (baseType == common::TypeRef::fragment()) {
                common::Fragment *fragment = project->fragment(projectItem->ref());
                if (fragment) {
                    common::Application::instance()->selection().select(fragment);
                }
            } else if (baseType == common::TypeRef::asset()) {
                common::Asset *asset = project->asset(projectItem->ref());
                if (asset) {
                    common::Application::instance()->selection().select(asset);
                }
            } else if (baseType == common::TypeRef::project()) {
                common::Application::instance()->selection().select(project);
            }
        }
    }
}

void WorkspaceDock::onProjectHubAdded(const common::LightRef &ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Hub *hub = workspace->findHub(ref);

    // add a new project hub
    if (hub) {
        projectModel->addHub(hub);
    }
}

void WorkspaceDock::onProjectHubRemoved(const common::LightRef &ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());

    // remove a project hub
    projectModel->removeHub(ref);
}

void WorkspaceDock::onProjectFragmentAdded(const o3d::studio::common::LightRef &ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Fragment *fragment = workspace->fragment(ref);

    // add a new project fragment
    if (fragment) {
        projectModel->addFragment(fragment);
    }
}

void WorkspaceDock::onProjectFragmentRemoved(const o3d::studio::common::LightRef &ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());

    // remove a project fragment
    projectModel->removeFragment(ref);
}

void WorkspaceDock::onProjectAssetAdded(const o3d::studio::common::LightRef &ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Asset *asset = workspace->asset(ref);

    // add a new project asset
    if (asset) {
        projectModel->addAsset(asset);
    }
}

void WorkspaceDock::onProjectAssetRemoved(const o3d::studio::common::LightRef &ref)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());

    // remove a project asset
    projectModel->removeAsset(ref);
}

void WorkspaceDock::focusInEvent(QFocusEvent *event)
{
    QDockWidget::focusInEvent(event);
}

void WorkspaceDock::onSelectManagerChange()
{
    common::TypeRef typeProject = common::Application::instance()->types().typeRef("o3s::project");

    const QSet<common::SelectionItem *> previousSelection = common::Application::instance()->selection().filterPrevious(typeProject.id());
    const QSet<common::SelectionItem *> currentSelection = common::Application::instance()->selection().filterCurrent(typeProject.id());

    common::SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, previousSelection) {

    }

    selectionItem = nullptr;
    foreach (selectionItem, currentSelection) {
    }

//    QModelIndex previous = m_treeView->currentIndex();
//    QModelIndex current = QModelIndex();

//    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
//    common::ProjectItem *projectItem = projectModel->find(ref);
//    if (projectItem) {
//        current = projectModel->index(projectItem->row(), 0);
//        m_treeView->selectionModel()->select(current, QItemSelectionModel::Current);
//    }
}

void WorkspaceDock::setupUi()
{
    setWindowIcon(QIcon::fromTheme("input-gaming"));

    m_treeView = new QTreeView(this);
    setWidget(m_treeView);

    m_treeView->setHeaderHidden(true);
    m_treeView->setModel(new common::ProjectModel());
}

void WorkspaceDock::onChangeCurrentWorkspace(const QString &name)
{
    QAbstractItemModel *oldModel = m_treeView->model();
    m_treeView->setModel(new common::ProjectModel());

    if (oldModel) {
        delete oldModel;
    }

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // workspace project
        connect(workspace, SIGNAL(onProjectAdded(const LightRef &)), SLOT(onAddProject(const LightRef &)));
        connect(workspace, SIGNAL(onProjectActivated(const LightRef &)), SLOT(onActivateProject(const LightRef &)));
        connect(workspace, SIGNAL(onProjectRemoved(const LightRef &)), SLOT(onRemoveProject(const LightRef &)));

        // project entities
        connect(workspace, SIGNAL(onProjectHubAdded(const LightRef &)), SLOT(onProjectHubAdded(const LightRef &)), Qt::QueuedConnection);
        connect(workspace, SIGNAL(onProjectHubRemoved(const LightRef &)), SLOT(onProjectHubRemoved(const LightRef &)), Qt::QueuedConnection);

        connect(workspace, SIGNAL(onProjectFragmentAdded(const LightRef &)), SLOT(onProjectFragmentAdded(const LightRef &)), Qt::QueuedConnection);
        connect(workspace, SIGNAL(onProjectFragmentRemoved(const LightRef &)), SLOT(onProjectFragmentRemoved(const LightRef &)), Qt::QueuedConnection);

        connect(workspace, SIGNAL(onProjectAssetAdded(const LightRef &)), SLOT(onProjectAssetAdded(const LightRef &)), Qt::QueuedConnection);
        connect(workspace, SIGNAL(onProjectAssetRemoved(const LightRef &)), SLOT(onProjectAssetRemoved(const LightRef &)), Qt::QueuedConnection);
    }
}

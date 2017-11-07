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
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/projectmodel.h"

#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"

#include <QtWidgets/QTreeView>

using namespace o3d::studio::main;


WorkspaceDock::WorkspaceDock(QWidget *parent) :
    QDockWidget(tr("Workspace"), parent),
    common::Dock()
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();

    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaceManager();
    connect(workspaceManager, SIGNAL(onWorkspaceActivated(QString)), SLOT(onChangeCurrentWorkspace(QString)));

    onChangeCurrentWorkspace(workspaceManager->current()->name());

    QItemSelectionModel *selectionModel = m_treeView->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSelectionChanged(const QModelIndex &, const QModelIndex &)));
    connect(m_treeView, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onSelectionDetails(const QModelIndex &)));
}

WorkspaceDock::~WorkspaceDock()
{

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

void WorkspaceDock::onAddProject(const QUuid &uuid)
{
    common::ProjectModel* projectModel = static_cast<common::ProjectModel*>(m_treeView->model());
    common::Project *project = common::Application::instance()->workspaceManager().current()->project(uuid);

    if (project) {
        projectModel->addProject(project);
    }
}

void WorkspaceDock::onRemoveProject(const QUuid &uuid)
{

}

void WorkspaceDock::onActivateProject(const QUuid &uuid)
{
    common::Workspace* workspace = common::Application::instance()->workspaceManager().current();
    common::Project *project = workspace->project(uuid);

    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::O3DCanvasContent *content = static_cast<common::O3DCanvasContent*>(
        uiCtrl.content(QString("o3s::main::o3dcanvascontent::") + project->uuid().toString()));

    if (content) {
        uiCtrl.setActiveContent(content, true);
    }
}

void WorkspaceDock::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(previous.internalPointer());
        // projectItem->unselect();
    }

    if (current.isValid()) {
        common::ProjectItem *projectItem = static_cast<common::ProjectItem*>(current.internalPointer());
        // projectItem->unselect();

        common::Project *project = projectItem->project();
        if (project) {
            project->workspace()->selectProject(project->uuid());
        }
    }
}

void WorkspaceDock::onSelectionDetails(const QModelIndex &)
{

}

void WorkspaceDock::setupUi()
{
    m_treeView = new QTreeView(this);
    setWidget(m_treeView);

    m_treeView->setHeaderHidden(true);
    m_treeView->setModel(new common::ProjectModel());
}

void WorkspaceDock::onChangeCurrentWorkspace(const QString &name)
{
    common::Workspace* workspace = common::Application::instance()->workspaceManager().current();
    if (workspace) {
        connect(workspace, SIGNAL(onProjectAdded(const QUuid &)), SLOT(onAddProject(const QUuid &)));
        connect(workspace, SIGNAL(onProjectActivated(const QUuid &)), SLOT(onActivateProject(const QUuid &)));
        connect(workspace, SIGNAL(onProjectRemoved(const QUuid &)), SLOT(onRemoveProject(const QUuid &)));
    }
}

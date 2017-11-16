/**
 * @brief Application main window main toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include "maintoolbar.h"
#include "mainwindow.h"

#include <QtWidgets/QLineEdit>
#include <QtCore/QList>
#include <QtGui/QPixmap>
#include <QtGui/QImage>

#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/command/hub/addhubcommand.h"
#include "o3d/studio/common/command/fragment/addfragmentcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"
#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"

#include "o3d/studio/common/ui/uiutils.h"

using namespace o3d::studio::main;


MainToolBar::MainToolBar(QWidget *parent) :
    QToolBar(tr("Main toolbar"), parent)
{
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);
    setupButtons();

    setWindowIcon(QIcon::fromTheme("applications-accessories"));

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    connect(workspaceManager, SIGNAL(onWorkspaceActivated(QString)), SLOT(onChangeCurrentWorkspace(QString)));

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

MainToolBar::~MainToolBar()
{

}

QToolBar *MainToolBar::ui()
{
    return this;
}

QString MainToolBar::elementName() const
{
    return("o3s::main::maintoolbar");
}

Qt::ToolBarArea MainToolBar::toolBarArea() const
{
    return Qt::TopToolBarArea;
}

void MainToolBar::onProjectActivated(const common::LightRef &ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = this->actions();
        QAction *action = nullptr;
        foreach (action, actionsList) {
            action->setEnabled(true);
        }
    }
}

void MainToolBar::onProjectRemoved(const common::LightRef &ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = this->actions();
        QAction *action = nullptr;
        foreach (action, actionsList) {
            action->setEnabled(false);
        }
    }
}

void MainToolBar::onCreateFragment()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            common::AddFragmentCommand *cmd = new common::AddFragmentCommand(project->ref().light(), QString());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void MainToolBar::onCreateHub()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        common::Selection &selection = common::Application::instance()->selection();

        // active project
        if (!project) {
            return;
        }

        const QSet<common::SelectionItem*> hubs = selection.filterCurrentByBaseType(common::TypeRef::hub());
        if (hubs.size() > 1) {
            return;
        }

        const common::Component *component = common::Application::instance()->components().component("o3s::common::component::dummyhub");
        if (!component) {
            return;
        }

        // add as sub-hub
        if (hubs.size() == 1) {
            auto it = hubs.begin();
            common::AddHubCommand *cmd = new common::AddHubCommand((*it)->ref(), component->typeRef(), QString());
            common::Application::instance()->command().addCommand(cmd);
        } else {
            common::AddHubCommand *cmd = new common::AddHubCommand(project->ref().light(), component->typeRef(), QString());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void MainToolBar::setupButtons()
{
    QAction *addFragment = new QAction(common::UiUtils::tintIcon(":/icons/fragment_flat.svg"), tr("Create a Fragment"));
    connect(addFragment, SIGNAL(triggered(bool)), SLOT(onCreateFragment()));
    addFragment->setEnabled(false);
    addAction(addFragment);

    QAction *addhub = new QAction(common::UiUtils::tintIcon(":/icons/device_hub_black.svg"), tr("Create a Hub"));
    connect(addhub, SIGNAL(triggered(bool)), SLOT(onCreateHub()));
    addhub->setEnabled(false);
    addAction(addhub);

    addSeparator();

    addAction(QIcon::fromTheme("system-search"), tr("Search"));
    addWidget(new QLineEdit());
}

void MainToolBar::onChangeCurrentWorkspace(const QString &name)
{
    Q_UNUSED(name)

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // connect(workspace, SIGNAL(onProjectAdded(const LightRef &)), SLOT(onProjectAdded(const LightRef &)));
        connect(workspace, SIGNAL(onProjectActivated(const LightRef &)), SLOT(onProjectActivated(const LightRef &)));
        connect(workspace, SIGNAL(onProjectRemoved(const LightRef &)), SLOT(onProjectRemoved(const LightRef &)));
    }
}

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
#include "o3d/studio/common/command/asset/addassetcommand.h"

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


MainToolBar::MainToolBar(BaseObject *parent) :
    BaseObject(parent),
    common::ToolBar()
{
    setupUi();

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &MainToolBar::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

MainToolBar::~MainToolBar()
{

}

QToolBar *MainToolBar::ui()
{
    return m_qtMainToolBar;
}

o3d::String MainToolBar::elementName() const
{
    return "o3s::main::maintoolbar";
}

Qt::ToolBarArea MainToolBar::toolBarArea() const
{
    return Qt::TopToolBarArea;
}

void MainToolBar::onProjectActivated(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtMainToolBar->actions();
        QAction *action = nullptr;
        foreach (action, actionsList) {
            action->setEnabled(true);
        }
    }
}

void MainToolBar::onProjectRemoved(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtMainToolBar->actions();
        QAction *action = nullptr;
        foreach (action, actionsList) {
            action->setEnabled(false);
        }
    }
}

void MainToolBar::setupUi()
{
    m_qtMainToolBar = new QtMainToolBar();
}

void MainToolBar::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectActivated.connect(this, &MainToolBar::onProjectActivated);
        workspace->onProjectRemoved.connect(this, &MainToolBar::onProjectRemoved);
        // workspace->onProjectAdded.connect(this, &MainToolBar::onProjectAdded);
    }
}

QtMainToolBar::QtMainToolBar(QWidget *parent) :
    QToolBar(tr("Main toolbar"), parent)
{
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);
    setupButtons();

    setWindowIcon(QIcon(":/icons/settings_applications_black.svg"));
}

QtMainToolBar::~QtMainToolBar()
{

}

void QtMainToolBar::onCreateAsset()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            common::AddAssetCommand *cmd = new common::AddAssetCommand(project->ref().light(), String());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void QtMainToolBar::onCreateFragment()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            common::AddFragmentCommand *cmd = new common::AddFragmentCommand(project->ref().light(), String());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void QtMainToolBar::onCreateHub()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        common::Selection &selection = common::Application::instance()->selection();

        // active project
        if (!project) {
            return;
        }

        const std::set<common::SelectionItem*> hubs = selection.filterCurrentByBaseType(common::TypeRef::hub());
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
            common::AddHubCommand *cmd = new common::AddHubCommand((*it)->ref(), component->typeRef(), String());
            common::Application::instance()->command().addCommand(cmd);
        } else {
            common::AddHubCommand *cmd = new common::AddHubCommand(project->ref().light(), component->typeRef(), String());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void QtMainToolBar::setupButtons()
{
    QAction *addFragment = new QAction(common::UiUtils::tintIcon(":/icons/fragment_flat.svg"), tr("Create a Fragment"));
    connect(addFragment, SIGNAL(triggered(bool)), SLOT(onCreateFragment()));
    addFragment->setEnabled(false);
    addAction(addFragment);

    QAction *addHub = new QAction(common::UiUtils::tintIcon(":/icons/device_hub_black.svg"), tr("Create a Hub"));
    connect(addHub, SIGNAL(triggered(bool)), SLOT(onCreateHub()));
    addHub->setEnabled(false);
    addAction(addHub);

    QAction *addAsset = new QAction(common::UiUtils::tintIcon(":/icons/archive_black.svg"), tr("Create an asset"));
    connect(addAsset, SIGNAL(triggered(bool)), SLOT(onCreateAsset()));
    addAsset->setEnabled(false);
    addAction(addAsset);

    addSeparator();

    addAction(QIcon::fromTheme("system-search"), tr("Search"));
    addWidget(new QLineEdit());
}

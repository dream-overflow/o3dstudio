/**
 * @brief Application main window main toolbar, contextual hub component toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include "hubcomponenttoolbar.h"
#include "../mainwindow.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtCore/QList>

#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/command/hub/addhubcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"
#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"

#include "o3d/studio/common/ui/uiutils.h"

using namespace o3d::studio::main;


HubComponentToolBar::HubComponentToolBar(const Point2i &pos, BaseObject *parent) :
    BaseObject(parent),
    m_pos(pos)
    // common::ToolBar()
{
    setupUi();

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &HubComponentToolBar::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

HubComponentToolBar::~HubComponentToolBar()
{
    // delete m_qtHubComponentToolBar;
}

void HubComponentToolBar::show()
{
    m_qtHubComponentToolBar->show();
}

//QToolBar *HubComponentToolBar::ui()
//{
//    return m_qtHubComponentToolBar;
//}

//o3d::String HubComponentToolBar::elementName() const
//{
//    return "o3s::main::toolbar::hubcomponent";
//}

//Qt::ToolBarArea HubComponentToolBar::toolBarArea() const
//{
//    return Qt::TopToolBarArea;
//}

void HubComponentToolBar::onProjectActivated(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtHubComponentToolBar->actions();
        for (QAction *action : actionsList) {
            action->setEnabled(true);
        }
    }
}

void HubComponentToolBar::onProjectRemoved(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtHubComponentToolBar->actions();
        for (QAction *action : actionsList) {
            action->setEnabled(false);
        }
    }
}

void HubComponentToolBar::setupUi()
{
    m_qtHubComponentToolBar = new QtHubComponentToolBar(QPoint(m_pos.x(), m_pos.y()));
}

void HubComponentToolBar::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectActivated.connect(this, &HubComponentToolBar::onProjectActivated);
        workspace->onProjectRemoved.connect(this, &HubComponentToolBar::onProjectRemoved);
    }
}

QtHubComponentToolBar::QtHubComponentToolBar(const QPoint &pos, QWidget *parent) :
    // QToolBar(tr("Hub component toolbar"), parent)
    QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::Popup);
    setWindowIcon(QIcon(":/icons/call_split_black.svg"));
    // setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);

    setupUi();
    move(pos);

    // @todo Set a border style
    // setStyle();
}

QtHubComponentToolBar::~QtHubComponentToolBar()
{

}

void QtHubComponentToolBar::onCreateHub()
{
    String name = fromQString(sender()->property("name").toString());

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

        const common::Component *component = common::Application::instance()->components().component(name);
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

    close();
    deleteLater();
}

void QtHubComponentToolBar::setupUi()
{
    QHBoxLayout *layout = new QHBoxLayout;
    QToolButton *action = nullptr;

    QSize iconSize(24, 24);

    // common::UiController &uiCtrl = common::Application::instance()->ui();
    auto components = common::Application::instance()->components().componentList();
    for (const common::Component* component : components) {
        action = new QToolButton;
        action->setProperty("name", toQString(component->name()));
        // action->setEnabled(false);

        action->setIcon(common::UiUtils::tintIcon(toQString(component->icon()), this));
        action->setIconSize(iconSize);
        action->setToolTip(toQString(component->label()));
        connect(action, SIGNAL(clicked(bool)), this, SLOT(onCreateHub()));
        layout->addWidget(action);
    }

    setLayout(layout);
}

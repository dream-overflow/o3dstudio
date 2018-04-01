/**
 * @brief Application main property dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#include "propertydock.h"
#include "../mainwindow.h"

#include <QtWidgets/QVBoxLayout>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::main;


PropertyDock::PropertyDock(BaseObject *parent) :
    BaseObject(parent),
    common::Dock()
{
    m_qtPropertyDock = new QtPropertyDock();
    m_qtPropertyDock->setupUi();

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &PropertyDock::onSelectionChanged);

    // ui ctrl
    common::Application::instance()->ui().attachPanel.connect(this, &PropertyDock::onAttachPanel);
    common::Application::instance()->ui().detachPanel.connect(this, &PropertyDock::onDetachPanel);

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &PropertyDock::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

PropertyDock::~PropertyDock()
{
    // deletePtr(m_qtPropertyDock);
}

QDockWidget *PropertyDock::ui()
{
    return m_qtPropertyDock;
}

o3d::String PropertyDock::elementName() const
{
    return "o3s::main::property";
}

Qt::DockWidgetArea PropertyDock::dockWidgetArea() const
{
    return Qt::RightDockWidgetArea;
}

void PropertyDock::onSelectionChanged()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::Project *project = nullptr;
    common::Component *component = nullptr;
    common::Panel *panel = nullptr;

    // project
    {
        const std::set<common::SelectionItem *> previousSelection =
                common::Application::instance()->selection().filterPreviousByBaseType(common::TypeRef::project());
        const std::set<common::SelectionItem *> currentSelection =
                common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::project());

        for (common::SelectionItem *selectionItem : previousSelection) {

        }

        for (common::SelectionItem *selectionItem : currentSelection) {

        }
    }

    // hub
    {
        const std::set<common::SelectionItem *> previousSelection =
                common::Application::instance()->selection().filterPreviousByBaseType(common::TypeRef::hub());
        const std::set<common::SelectionItem *> currentSelection =
                common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::hub());

        common::Hub *hub = nullptr;

        for (common::SelectionItem *selectionItem : previousSelection) {
            auto it = m_panels.find(selectionItem->ref());
            if (it == m_panels.end()) {
                continue;
            }

            panel = it->second;
            m_panels.erase(it);

            // sync data if the hub and the project still exists
            project = workspace->project(selectionItem->ref());
            if (project) {
                hub = static_cast<common::Hub*>(project->lookup(selectionItem->ref()));
                if (hub) {
                    panel->commit();
                }
            }

            delete panel;

            QString objName = toQString(selectionItem->ref().longId());

            for (int i = 0; i < m_qtPropertyDock->container()->count(); ++i) {
                QWidget *ui = m_qtPropertyDock->container()->itemAt(i)->widget();
                if (ui->objectName() == objName) {
                    m_qtPropertyDock->container()->removeWidget(ui);
                    ui->deleteLater();
                }
            }
        }

        for (common::SelectionItem *selectionItem : currentSelection) {
            project = workspace->project(selectionItem->ref());

            // we don't want a panel for a select of an undefined project
            if (!project) {
                return;
            }

            // neither for an undefined hub
            hub = static_cast<common::Hub*>(project->lookup(selectionItem->ref()));
            if (!hub) {
                continue;
            }

            component = common::Application::instance()->components().componentByTarget(hub->ref().strong().typeName());
            if (!component) {
                continue;
            }

            panel = component->panel(common::Panel::PANEL_PROPERTY, hub);
            if (panel) {
                QWidget *ui = panel->ui();
                ui->setObjectName(toQString(hub->ref().light().longId()));

                m_qtPropertyDock->container()->addWidget(ui);
                m_panels[hub->ref().light()] = panel;

                panel->update();
            }
        }
    }
}

void PropertyDock::onAttachPanel(o3d::String name, common::Panel *panel)
{
    // @todo sync previous one

    // only interested by property panels
    if (panel->panelType() == common::Panel::PANEL_PROPERTY) {
        m_qtPropertyDock->container()->addWidget(panel->ui());
    }
}

void PropertyDock::onDetachPanel(o3d::String name, common::Panel *panel)
{
    // @todo sync it before

    // only interested by property panels
    if (panel->panelType() == common::Panel::PANEL_PROPERTY) {
        m_qtPropertyDock->container()->removeWidget(nullptr);
        panel->ui()->setParent(nullptr);
    }
}

void PropertyDock::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // workspace project
//        workspace->onProjectAdded.connect(this, &WorkspaceDock::onAddProject);
//        workspace->onProjectRemoved.connect(this, &WorkspaceDock::onRemoveProject);
//        workspace->onProjectActivated.connect(this, &WorkspaceDock::onActivateProject);

//        // project entities
//        workspace->onProjectHubAdded.connect(this, &WorkspaceDock::onProjectHubAdded, CONNECTION_ASYNCH);
//        workspace->onProjectHubRemoved.connect(this, &WorkspaceDock::onProjectHubRemoved, CONNECTION_ASYNCH);

//        workspace->onProjectFragmentAdded.connect(this, &WorkspaceDock::onProjectFragmentAdded, CONNECTION_ASYNCH);
//        workspace->onProjectFragmentRemoved.connect(this, &WorkspaceDock::onProjectFragmentRemoved, CONNECTION_ASYNCH);

        workspace->onProjectEntityChanged.connect(this, &PropertyDock::onProjectEntityChanged, CONNECTION_ASYNCH);
    }
}

void PropertyDock::onProjectEntityChanged(common::LightRef ref, o3d::BitSet64 changeFlags)
{
    if (changeFlags.getBit(common::Entity::MODEL_CHANGED)) {
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        common::Panel *panel = nullptr;

        // find the panel and update it
        auto it = m_panels.find(ref);
        if (it == m_panels.end()) {
            return;
        }

        panel = it->second;

        if (panel) {
            panel->update();
        }
    }
}

QtPropertyDock::QtPropertyDock(QWidget *parent) :
    QDockWidget(tr("Property"), parent),
    m_container(nullptr)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
}

QtPropertyDock::~QtPropertyDock()
{

}

QLayout *QtPropertyDock::container()
{
    return m_container->layout();
}

void QtPropertyDock::setupUi()
{
    m_container = new QWidget();
    m_container->setLayout(new QVBoxLayout());
    m_container->layout()->setAlignment(Qt::AlignTop);
    m_container->layout()->setContentsMargins(0, 2, 0, 2);

    QScrollArea *scrollArea = new QScrollArea();
    // scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(m_container);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);

    setWidget(scrollArea);

    setWindowIcon(QIcon(":/icons/settings_input_component_black.svg"));
}

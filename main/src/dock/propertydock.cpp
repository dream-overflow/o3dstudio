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
    setupUi();

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &PropertyDock::onSelectionChanged);

    // ui ctrl
    common::Application::instance()->ui().attachPanel.connect(this, &PropertyDock::onAttachPanel);
    common::Application::instance()->ui().detachPanel.connect(this, &PropertyDock::onDetachPanel);
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
            // @todo sync data if necessary, remove widget

            project = workspace->project(selectionItem->ref());

            if (!project) {
                return;
            }

            hub = project->rootHub()->findHub(selectionItem->ref().id());
            if (!hub) {
                continue;
            }

            component = common::Application::instance()->components().componentByTarget(
                            hub->ref().strong().typeName());
            if (!component) {
                continue;
            }

            // @todo what if multiple ? bulk or list ?
            auto it = m_panels.find(hub->ref().light());
            if (it == m_panels.end()) {
                continue;
            }

            panel = it->second;
            m_panels.erase(it);

            // @todo sync data before delete or done by dtor...
            delete panel;

            QString objName = toQString(hub->ref().light().longId());

            for (int i = 0; i < m_qtPropertyDock->widget()->layout()->count(); ++i) {
                QWidget *ui = m_qtPropertyDock->widget()->layout()->itemAt(i)->widget();
                if (ui->objectName() == objName) {
                    m_qtPropertyDock->widget()->layout()->removeWidget(ui);
                    ui->deleteLater();
                }
            }
        }

        for (common::SelectionItem *selectionItem : currentSelection) {
            project = workspace->project(selectionItem->ref());

            if (!project) {
                return;
            }

            hub = project->rootHub()->findHub(selectionItem->ref().id());
            if (!hub) {
                continue;
            }

            component = common::Application::instance()->components().componentByTarget(
                            hub->ref().strong().typeName());
            if (!component) {
                continue;
            }

            // @todo what if multiple ? bulk or list ?
            panel = component->panel(common::Panel::PANEL_PROPERTY, hub);
            if (panel) {
                QWidget *ui = panel->ui();
                ui->setObjectName(toQString(hub->ref().light().longId()));

                m_qtPropertyDock->widget()->layout()->addWidget(ui);
                m_panels[hub->ref().light()] = panel;
                // @todo cleanup
            }
        }
    }
}

void PropertyDock::onAttachPanel(o3d::String name, common::Panel *panel)
{
    // @todo sync previous one

    // only interested by property panels
    if (panel->panelType() == common::Panel::PANEL_PROPERTY) {
        m_qtPropertyDock->setWidget(panel->ui());
    }
}

void PropertyDock::onDetachPanel(o3d::String name, common::Panel *panel)
{
    // @todo sync it before

    // only interested by property panels
    if (panel->panelType() == common::Panel::PANEL_PROPERTY) {
        m_qtPropertyDock->setWidget(nullptr);
        panel->ui()->setParent(nullptr);
    }
}

void PropertyDock::setupUi()
{
    m_qtPropertyDock = new QtPropertyDock();

    QWidget *widget = new QWidget();
    widget->setLayout(new QVBoxLayout());
    widget->layout()->setAlignment(Qt::AlignTop);
    widget->layout()->setContentsMargins(0, 2, 0, 2);

    m_qtPropertyDock->setWidget(widget);
}

QtPropertyDock::QtPropertyDock(QWidget *parent) :
    QDockWidget(tr("Property"), parent)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
}

QtPropertyDock::~QtPropertyDock()
{

}

void QtPropertyDock::setupUi()
{
    setWindowIcon(QIcon(":/icons/settings_input_component_black.svg"));
}

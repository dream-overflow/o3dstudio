/**
 * @brief Application common transform dock related to current transform manipulator
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-04-22
 * @details
 */

#include "o3d/studio/common/ui/dock/transformdock.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/selection.h"

#include "o3d/studio/common/global.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/ui/scene/hubmanipulator.h"

#include "o3d/studio/common/ui/uiutils.h"
#include "o3d/studio/common/ui/panelbuilder.h"
#include "o3d/studio/common/ui/property/vector3property.h"

#include <o3d/engine/object/transform.h>

using namespace o3d::studio::common;

TransformDock::TransformDock(BaseObject *parent) :
    BaseObject(parent),
    common::Dock(),
    m_panel(nullptr)
{
    m_qtTransformDock = new QtTransformDock();
    m_qtTransformDock->setupUi();

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &TransformDock::onSelectionChanged);

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &TransformDock::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

TransformDock::~TransformDock()
{
    // deletePtr(m_qtTransformDock);
}

QDockWidget *TransformDock::ui()
{
    return m_qtTransformDock;
}

o3d::String TransformDock::elementName() const
{
    return "o3s::main::common::transform";
}

Qt::DockWidgetArea TransformDock::dockWidgetArea() const
{
    return Qt::RightDockWidgetArea;
}

void TransformDock::onSelectionChanged()
{
    Workspace* workspace = Application::instance()->workspaces().current();
    if (!workspace) {
        return;
    }

    Project *project = workspace->activeProject();
    if (!project) {
        return;
    }

    if (Application::instance()->selection().currentSelection().empty()) {
        return;
    }

    if (Application::instance()->selection().acceptedRole() != Selection::ACCEPT_STRUCTURAL_HUB) {
        return;
    }

    if (m_panel) {
        m_panel->update();
    } else {
        setupPanel();
        m_panel->update();
    }
}

void TransformDock::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // remove panel
        cleanupPanel();

        // workspace project @todo and master scene if fragment ?
        workspace->onProjectActivated.connect(this, &TransformDock::onActivateProject, CONNECTION_ASYNCH);
        workspace->onProjectFragmentActivated.connect(this, &TransformDock::onActivateFragment, CONNECTION_ASYNCH);

        workspace->onProjectEntityChanged.connect(this, &TransformDock::onProjectEntityChanged, CONNECTION_ASYNCH);
    }
}

void TransformDock::setupPanel()
{
    if (!m_panel) {
        m_panel = new TransformPanel();
        m_qtTransformDock->container()->addWidget(m_panel->ui());
    }
}

void TransformDock::cleanupPanel()
{
    // remove panel
    for (int i = 0; i < m_qtTransformDock->container()->count(); ++i) {
        QWidget *ui = m_qtTransformDock->container()->itemAt(i)->widget();
        m_qtTransformDock->container()->removeWidget(ui);
        ui->deleteLater();
    }

    m_panel = nullptr;
}

void TransformDock::onProjectEntityChanged(common::LightRef ref, o3d::BitSet64 changeFlags)
{
    if (changeFlags.getBit(common::Entity::MODEL_CHANGED)) {
        common::Workspace* workspace = common::Application::instance()->workspaces().current();

        // a change occurs on a selected entity update panel values
        if (workspace->entity(ref)->isSelected()) {
            if (!m_panel) {
                setupPanel();
            } else {
                m_panel->update();
            }
        }
    }
}

void TransformDock::onActivateProject(LightRef ref)
{
    O3D_UNUSED(ref);
}

void TransformDock::onActivateFragment(LightRef ref)
{
    O3D_UNUSED(ref);
}

QtTransformDock::QtTransformDock(QWidget *parent) :
    QDockWidget(tr("Current transform"), parent),
    m_container(nullptr)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
}

QtTransformDock::~QtTransformDock()
{

}

QLayout *QtTransformDock::container()
{
    return m_container->layout();
}

void QtTransformDock::setupUi()
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

    setWindowIcon(QIcon("://icons/center_focus_weak_black.svg"));
}

TransformPanel::TransformPanel() :
    m_manualMode(0)
{

}

TransformPanel::~TransformPanel()
{

}

o3d::String TransformPanel::elementName() const
{
    return "o3s::common::component::panel::transform";
}

QWidget *TransformPanel::ui()
{
    PanelBuilder pb(this, fromQString(tr("Current transform")));

    m_position = new Vector3Property(this, "position", fromQString(tr("Position")));
    pb.addPanelProperty(m_position);

    m_position->onValueChanged.connect(this, [this] (Vector3) {
        m_manualMode = 1;
        commit();
    });

    m_rotation = new Vector3Property(this, "rotation", fromQString(tr("Rotation")));
    pb.addPanelProperty(m_rotation);

    m_rotation->onValueChanged.connect(this, [this] (Vector3) {
        m_manualMode = 2;
        commit();
    });

    m_scale = new Vector3Property(this, "scale", fromQString(tr("Scale")));
    pb.addPanelProperty(m_scale);

    m_scale->onValueChanged.connect(this, [this] (Vector3) {
        m_manualMode = 3;
        commit();
    });

    // initial scale
    m_scale->setValue(Vector3f(1,1,1));

    return pb.ui();
}

Panel::PanelType TransformPanel::panelType() const
{
    return PANEL_PROPERTY;
}

void TransformPanel::commit()
{
    Workspace* workspace = Application::instance()->workspaces().current();
    if (!workspace) {
        return;
    }

    Project* project = workspace->activeProject();
    if (!project) {
        return;
    }

    if (project->masterScene() && project->masterScene()->hubManipulator()) {
        HubManipulator *hubManipulator = static_cast<HubManipulator*>(project->masterScene()->hubManipulator());

        if (m_manualMode == 1) {
            hubManipulator->setPosition(m_position->value());
        } else if (m_manualMode == 2) {
            hubManipulator->setRotation(m_rotation->value());
        } else if (m_manualMode == 3) {
            hubManipulator->setScale(m_scale->value());
        }
    }
}

void TransformPanel::update()
{
    Workspace* workspace = Application::instance()->workspaces().current();
    if (!workspace) {
        return;
    }

    Project* project = workspace->activeProject();
    if (!project) {
        return;
    }

    // @todo for now use the master scene of the project but need an active master scene
    // or a way to have a connection to the hub manipulator
    if (project->masterScene() && project->masterScene()->hubManipulator()) {
        const Transform *transform = static_cast<const HubManipulator*>(project->masterScene()->hubManipulator())->currentTransform();

        Vector3 euler;
        transform->getRotation().toEuler(euler);
        // @todo range clamp...

        m_position->setValue(transform->getPosition());
        m_rotation->setValue(euler);
        m_scale->setValue(transform->getScale());
    }
}

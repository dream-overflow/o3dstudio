/**
 * @brief Add hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-11
 * @details
 */

#include <QtWidgets/QApplication>

#include "o3d/studio/common/command/hub/addhubcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;


AddHubCommand::AddHubCommand(const LightRef &project, const QString &name) :
    Command("o3s::common::hub::add"),
    m_project(project),
    m_hubName(name)
{
    if (m_hubName.isEmpty()) {
        m_hubName = "Unamed hub";
    }

    Q_ASSERT(m_project.isValid());
}

AddHubCommand::~AddHubCommand()
{
}

QString AddHubCommand::commandLabel() const
{
    return tr("Add a hub");
}

bool AddHubCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Hub *hub = new Hub(m_hubName, project);
            project->addHub(hub);

            m_hub = hub->ref().light();
            return true;
        }
    }

    return false;
}

bool AddHubCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            project->removeHub(m_hub);
            return true;
        }
    }

    return false;
}

bool AddHubCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Hub *hub = new Hub(m_hubName, project);
            project->addHub(hub);

            m_hub = hub->ref().light();
            return true;
        }
    }

    return false;
}

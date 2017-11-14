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


AddHubCommand::AddHubCommand(const LightRef &parent, const QString &name) :
    Command("o3s::common::hub::add", parent),
    m_parent(parent),
    m_hubName(name)
{
    if (m_hubName.isEmpty()) {
        m_hubName = "Unamed hub";
    }

    Q_ASSERT(m_parent.isValid());
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
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parent.type() == TypeRef::project().id()) {
            Hub *hub = new Hub(m_hubName, project);
            // with new ref id
            hub->setRef(ObjectRef::buildRef(project, TypeRef::hub()));

            project->addHub(hub);

            m_hub = hub->ref();
            return true;
        } else if (project && m_parent.type() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = new Hub(m_hubName, parentHub);
                // with new ref id
                hub->setRef(ObjectRef::buildRef(project, TypeRef::hub()));

                parentHub->addHub(hub);

                m_hub = hub->ref();
                return true;
            }
        }
    }

    return false;
}

bool AddHubCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parent.type() == TypeRef::project().id()) {
            project->removeHub(m_hub.light());
            return true;
        } else if (project && m_parent.type() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                parentHub->removeHub(m_hub.light());
                return true;
            }
        }
    }

    return false;
}

bool AddHubCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parent.type() == TypeRef::project().id()) {
            Hub *hub = new Hub(m_hubName, project);
            // reuse ref id
            hub->setRef(m_hub);

            project->addHub(hub);
            return true;
        } else if (project && m_parent.type() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = new Hub(m_hubName, parentHub);
                // reuse ref id
                hub->setRef(m_hub);

                parentHub->addHub(hub);
                return true;
            }
        }
    }

    return false;
}
/**
 * @brief Remove hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-14
 * @details
 */

#include <QtWidgets/QApplication>
#include <QtCore/QDataStream>

#include "o3d/studio/common/command/hub/removehubcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;


RemoveHubCommand::RemoveHubCommand(const LightRef &hubRef, const LightRef &parentRef, const TypeRef &parentTypeRef) :
    Command("o3s::common::hub::remove", hubRef),
    m_parent(parentRef),
    m_hub(hubRef),
    m_parentTypeRef(parentTypeRef)
{
    Q_ASSERT(m_parent.isValid());
    Q_ASSERT(m_hub.isValid());
}

RemoveHubCommand::~RemoveHubCommand()
{
}

QString RemoveHubCommand::commandLabel() const
{
    return tr("Remove a hub");
}

bool RemoveHubCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parentTypeRef.baseType() == TypeRef::project().id()) {
            Hub *hub = project->hub(m_hub);
            if (hub) {
                // backup
                QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                stream << *hub;
                m_storedHubRef = hub->ref();

                project->removeHub(m_hub);
                return true;
            }
        } else if (project && m_parentTypeRef.baseType() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = parentHub->hub(m_hub);
                if (hub) {
                    // backup
                    QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                    stream << *hub;
                    m_storedHubRef = hub->ref();

                    parentHub->removeHub(m_hub);
                    return true;
                }
            }
        }
    }

    return false;
}

bool RemoveHubCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parentTypeRef.baseType() == TypeRef::project().id()) {
            Hub *hub = new Hub("", project);

            // restore content
            QDataStream stream(&m_data, QIODevice::ReadOnly);
            stream >> *hub;
            hub->setRef(m_storedHubRef);

            m_data.clear();

            project->addHub(hub);
            return true;
        } else if (project && m_parentTypeRef.baseType() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = new Hub("", parentHub);

                // restore content
                QDataStream stream(&m_data, QIODevice::ReadOnly);
                stream >> *hub;
                hub->setRef(m_storedHubRef);

                m_data.clear();

                parentHub->addHub(hub);
                return true;
            }
        }
    }

    return false;
}

bool RemoveHubCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level hub, direct to project
        if (project && m_parentTypeRef.baseType() == TypeRef::project().id()) {
            Hub *hub = project->hub(m_hub);
            if (hub) {
                // backup
                QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                stream << *hub;
                m_storedHubRef = hub->ref();

                project->removeHub(m_hub);
                return true;
            }
        } else if (project && m_parentTypeRef.baseType() == TypeRef::hub().id()) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = parentHub->hub(m_hub);
                if (hub) {
                    // backup
                    QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                    stream << *hub;
                    m_storedHubRef = hub->ref();

                    parentHub->removeHub(m_hub);
                    return true;
                }
            }
        }
    }

    return false;
}

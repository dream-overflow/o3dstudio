/**
 * @brief Remove hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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
#include "o3d/studio/common/workspace/hub.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/componentregistry.h"

#include <o3d/core/datainstream.h>
#include <o3d/core/dataoutstream.h>

using namespace o3d::studio::common;


RemoveHubCommand::RemoveHubCommand(const LightRef &hubRef, const LightRef &parentRef) :
    Command("o3s::common::hub::remove", hubRef),
    m_parent(parentRef),
    m_hub(hubRef),
    m_nodePos(-1)
{
    O3D_ASSERT(m_parent.isValid());
    O3D_ASSERT(m_hub.isValid());
}

RemoveHubCommand::~RemoveHubCommand()
{
}

o3d::String RemoveHubCommand::commandLabel() const
{
    return fromQString(tr("Remove a hub"));
}

o3d::Bool RemoveHubCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = parentHub->hub(m_hub);
                if (hub) {
                    // backup
                    DataOutStream stream(m_data);
                    stream << *hub;
                    m_storedHubRef = hub->ref();
                    m_nodePos = parentHub->childIndexOf(hub);

                    parentHub->removeHub(m_hub);
                    return True;
                }
            }
        }
    }

    return False;
}

o3d::Bool RemoveHubCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        Component *component = Application::instance()->components().componentByTarget(m_storedHubRef.strong().typeName());
        if (!component) {
            return False;
        }

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = component->buildHub("", project, parentHub);
                hub->setProject(project);

                // restore content
                DataInStream stream(m_data);
                hub->setRef(m_storedHubRef);

                parentHub->addHub(hub, m_nodePos);

                stream >> *hub;

                m_data.destroy();

                // hub was added after
//                project->workspace()->onProjectHubAdded(hub->ref().light());

//                // iterator over children
//                common::Hub *node = nullptr;
//                foreach (node, hub->hubs(true)) {
//                    // signal throught project->workspace
//                    project->workspace()->onProjectHubAdded(node->ref().light());
//                }

                return True;
            }
        }
    }

    return False;
}

o3d::Bool RemoveHubCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = parentHub->hub(m_hub);
                if (hub) {
                    // backup
                    DataOutStream stream(m_data);
                    stream << *hub;
                    m_storedHubRef = hub->ref();

                    parentHub->removeHub(m_hub);
                    return True;
                }
            }
        }
    }

    return False;
}

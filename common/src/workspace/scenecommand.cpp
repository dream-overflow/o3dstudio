/**
 * @brief Common scene command class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-16
 * @details
 */

#include "o3d/studio/common/workspace/scenecommand.h"
#include "o3d/studio/common/workspace/hub.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;


SceneCommand::~SceneCommand()
{

}


SceneHubCommand::SceneHubCommand(Hub *hub, SceneHubCommand::CommandType cmdType) :
    m_hub(hub)
{
    O3D_ASSERT(hub != nullptr);

    if (m_hub) {
        m_ref = m_hub->ref().light();
    }
}

SceneHubCommand::~SceneHubCommand()
{

}

void SceneHubCommand::process(MasterScene *masterScene)
{
    if (m_ref.isValid()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        const Project *project = workspace->project(m_ref);

        if (!project) {
            // invalid or deleted project
            return;
        }

        if (!project->lookup(m_ref)) {
            // invalid or deleted hub
            return;
        }
    }

    if (m_cmdType == CREATE) {
        m_hub->createToScene(masterScene);
    } else if (m_cmdType == DELETE) {
        m_hub->removeFromScene(masterScene);
    } else if (m_cmdType == SYNC) {
        m_hub->syncWithScene(masterScene);
    }
}

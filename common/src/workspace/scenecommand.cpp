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

#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::common;


SceneCommand::~SceneCommand()
{

}


SceneHubCommand::SceneHubCommand(Hub *hub, SceneHubCommand::CommandType cmdType) :
    m_cmdType(cmdType),
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

        if (m_cmdType == CREATE) {
            if (!project->lookup(m_ref)) {
                // normaly added into project before this command
                return;
            }

            try {
                m_hub->createToScene(masterScene);
            } catch (E_BaseException &e) {
                Application::instance()->messenger().critical(e.getMsg());
            }
        } else if (m_cmdType == DELETE) {
            try {
                m_hub->removeFromScene(masterScene);
            } catch (E_BaseException &e) {
                Application::instance()->messenger().critical(e.getMsg());
            }
        } else if (m_cmdType == SYNC) {
            if (!project->lookup(m_ref)) {
                // normaly deleted from the project brefore this command
                return;
            }

            try {
                m_hub->syncWithScene(masterScene);
            } catch (E_BaseException &e) {
                Application::instance()->messenger().critical(e.getMsg());
            }
        }
    } else {
        // the hub can be removed but still not deleted, then we just interest in the DELETE command
        if (m_cmdType == DELETE) {
            try {
                m_hub->removeFromScene(masterScene);
            } catch (E_BaseException &e) {
                Application::instance()->messenger().critical(e.getMsg());
            }
        }
    }
}

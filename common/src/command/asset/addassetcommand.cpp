/**
 * @brief Add asset command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#include "o3d/studio/common/command/asset/addassetcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;


AddAssetCommand::AddAssetCommand(const LightRef &project, const String &name) :
    Command("o3s::common::asset::add", project),
    m_project(project),
    m_assetName(name)
{
    if (m_assetName.isEmpty()) {
        m_assetName = "Unamed asset";
    }

    O3D_ASSERT(m_project.isValid());
}

AddAssetCommand::~AddAssetCommand()
{
}

o3d::String AddAssetCommand::commandLabel() const
{
    return fromQString(tr("Add an asset"));
}

o3d::Bool AddAssetCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Asset *asset = new Asset(m_assetName, project);
            // with new ref id
            asset->setRef(ObjectRef::buildRef(project, TypeRef::asset()));

            // @todo library
            // project->addAsset(asset);

            m_asset = asset->ref();
            return True;
        }
    }

    return False;
}

o3d::Bool AddAssetCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            // @todo library
            //project->removeAsset(m_asset);
            return True;
        }
    }

    return False;
}

o3d::Bool AddAssetCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Asset *asset = new Asset(m_assetName, project);
            // reuse ref id
            asset->setRef(m_asset);

            // @todo library
            // project->addAsset(asset);

            return True;
        }
    }

    return False;
}

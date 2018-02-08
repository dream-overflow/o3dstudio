/**
 * @brief Remove asset command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#include <QtWidgets/QApplication>

#include "o3d/studio/common/command/asset/removeassetcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/asset.h"

#include <o3d/core/datainstream.h>
#include <o3d/core/dataoutstream.h>

using namespace o3d::studio::common;


RemoveAssetCommand::RemoveAssetCommand(const LightRef &assetRef, const LightRef &parentRef) :
    Command("o3s::common::asset::remove", assetRef),
    m_parent(parentRef),
    m_asset(assetRef)
{
    O3D_ASSERT(m_parent.isValid());
    O3D_ASSERT(m_asset.isValid());
}

RemoveAssetCommand::~RemoveAssetCommand()
{
}

o3d::String RemoveAssetCommand::commandLabel() const
{
    return fromQString(tr("Remove an asset"));
}

o3d::Bool RemoveAssetCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level asset, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            // @todo
//            Asset *asset = project->asset(m_asset);
//            if (asset) {
//                // backup
//                DataOutStream stream(m_data);
//                stream << *asset;
//                m_storedAssetRef = asset->ref();

//                project->removeAsset(m_asset);
//                return True;
//            }
        }
    }

    return False;
}

o3d::Bool RemoveAssetCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level asset, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            // @todo
//            Asset *asset = new Asset("", project);
//            asset->setProject(project);

//            // restore content
//            DataInStream stream(m_data);
//            stream >> *asset;
//            asset->setRef(m_storedAssetRef);

//            m_data.destroy();

//            project->addAsset(asset);

//            return True;
        }
    }

    return False;
}

o3d::Bool RemoveAssetCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level asset, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            // @todo
//            Asset *asset = project->asset(m_asset);
//            if (asset) {
//                // backup
//                DataOutStream stream(m_data);
//                stream << *asset;
//                m_storedAssetRef = asset->ref();

//                project->removeAsset(m_asset);
//                return True;
//            }
        }
    }

    return False;
}

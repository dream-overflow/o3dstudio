/**
 * @brief Add hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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
#include "o3d/studio/common/workspace/asset.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/componentregistry.h"

using namespace o3d::studio::common;


AddHubCommand::AddHubCommand(const LightRef &parent, const TypeRef& componentType, const String &name) :
    Command("o3s::common::hub::add", parent),
    m_componentType(componentType),
    m_parent(parent),
    m_hubName(name)
{
    if (m_hubName.isEmpty()) {
        m_hubName = "Unamed hub";
    }

    O3D_ASSERT(m_parent.isValid());
}

AddHubCommand::~AddHubCommand()
{
}

o3d::String AddHubCommand::commandLabel() const
{
    return fromQString(tr("Add a hub"));
}

#include "o3d/studio/common/importer/importerregistry.h"
#include "o3d/studio/common/importer/importer.h"
#include "o3d/studio/common/importer/importdefinition.h"

o3d::Bool AddHubCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        Component *component = Application::instance()->components().component(m_componentType);
        if (!component) {
            return False;
        }

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = component->buildHub(m_hubName, project, parentHub);
                // with new ref id
                hub->setRef(ObjectRef::buildRef(project, hub->typeRef()));

                parentHub->addHub(hub);

                m_storedHubRef = hub->ref();

                // dev only test to import FBX @todo remove me after
                Importer *fbxi = Application::instance()->importers().importer("o3s::plugin::importer::fbxi");
                ImportDefinition *def = fbxi->import(
                                            "../../../../test/fbx/shaderBall/shaderBall.fbx",
                                            new ImporterOption(),
                                            hub);

                deletePtr(def);

                return True;
            }
        }
    }

    return False;
}

o3d::Bool AddHubCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                parentHub->removeHub(m_storedHubRef.light());
                return True;
            }
        }
    }

    return False;
}

o3d::Bool AddHubCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        Component *component = Application::instance()->components().component(m_componentType);
        if (!component) {
            return False;
        }

        if (project && m_parent.baseTypeOf(TypeRef::hub())) {
            Hub *parentHub = workspace->findHub(m_parent);
            if (parentHub) {
                Hub *hub = component->buildHub(m_hubName, project, parentHub);
                // reuse ref id
                hub->setRef(m_storedHubRef);

                parentHub->addHub(hub);
                return True;
            }
        }
    }

    return False;
}

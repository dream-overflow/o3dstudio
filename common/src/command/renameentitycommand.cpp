/**
 * @brief Rename entity command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#include <QtWidgets/QApplication>

#include "o3d/studio/common/command/renameentitycommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/componentregistry.h"

using namespace o3d::studio::common;


RenameEntityCommand::RenameEntityCommand(const LightRef &entityRef, const LightRef &parent, const String &name) :
    Command("o3s::common::entity::rename", entityRef),
    m_parent(parent),
    m_entityName(name)
{
    if (m_entityName.isEmpty()) {
        m_entityName = "Unamed hub";
    }

    if (!entityRef.baseTypeOf(TypeRef::project())) {
        O3D_ASSERT(m_parent.isValid());
    }
}

RenameEntityCommand::~RenameEntityCommand()
{
}

o3d::String RenameEntityCommand::commandLabel() const
{
    return fromQString(tr("Rename an entity"));
}

o3d::Bool RenameEntityCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project) {
            Entity *entity = project->lookup(m_targetRef);
            if (entity) {
                m_storedName = entity->name();
                entity->setName(m_entityName);

                BitSet64 changes;
                changes.enable(Entity::PRESENTATION_CHANGED);
                project->workspace()->onProjectEntityChanged(m_targetRef, changes);

                return True;
            }
        }
    }

    return False;
}

o3d::Bool RenameEntityCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project) {
            Entity *entity = project->lookup(m_targetRef);
            if (entity) {
                entity->setName(m_storedName);

                BitSet64 changes;
                changes.enable(Entity::PRESENTATION_CHANGED);
                project->workspace()->onProjectEntityChanged(m_targetRef, changes);

                return True;
            }
        }
    }

    return False;
}

o3d::Bool RenameEntityCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        if (project) {
            Entity *entity = project->lookup(m_targetRef);
            if (entity) {
                entity->setName(m_entityName);

                BitSet64 changes;
                changes.enable(Entity::PRESENTATION_CHANGED);
                project->workspace()->onProjectEntityChanged(m_targetRef, changes);

                return True;
            }
        }
    }

    return False;
}

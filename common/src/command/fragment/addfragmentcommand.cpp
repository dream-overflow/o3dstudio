/**
 * @brief Add fragment command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include "o3d/studio/common/command/fragment/addfragmentcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;


AddFragmentCommand::AddFragmentCommand(const LightRef &project, const String &name) :
    Command("o3s::common::fragment::add", project),
    m_project(project),
    m_fragmentName(name)
{
    if (m_fragmentName.isEmpty()) {
        m_fragmentName = "Unamed fragment";
    }

    O3D_ASSERT(m_project.isValid());
}

AddFragmentCommand::~AddFragmentCommand()
{
}

o3d::String AddFragmentCommand::commandLabel() const
{
    return fromQString(tr("Add a fragment"));
}

o3d::Bool AddFragmentCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Fragment *fragment = new Fragment(m_fragmentName, project);
            // with new ref id
            fragment->setRef(ObjectRef::buildRef(project, TypeRef::fragment()));

            project->addFragment(fragment);

            m_fragment = fragment->ref();
            return True;
        }
    }

    return False;
}

o3d::Bool AddFragmentCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            project->removeFragment(m_fragment);
            return True;
        }
    }

    return False;
}

o3d::Bool AddFragmentCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Fragment *fragment = new Fragment(m_fragmentName, project);
            // reuse ref id
            fragment->setRef(m_fragment);

            project->addFragment(fragment);
            return True;
        }
    }

    return False;
}

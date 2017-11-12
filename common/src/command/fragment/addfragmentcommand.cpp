/**
 * @brief Add fragment command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-11
 * @details
 */

#include <QtWidgets/QApplication>

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


AddFragmentCommand::AddFragmentCommand(const LightRef &project, const QString &name) :
    Command("o3s::common::fragment::add"),
    m_project(project),
    m_fragmentName(name)
{
    if (m_fragmentName.isEmpty()) {
        m_fragmentName = "Unamed fragment";
    }

    Q_ASSERT(m_project.isValid());
}

AddFragmentCommand::~AddFragmentCommand()
{
}

QString AddFragmentCommand::commandLabel() const
{
    return tr("Add a fragment");
}

bool AddFragmentCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Fragment *fragment = new Fragment(m_fragmentName, project);
            project->addFragment(fragment);

            m_fragment = fragment->ref().light();
            return true;
        }
    }

    return false;
}

bool AddFragmentCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            project->removeFragment(m_fragment);
            return true;
        }
    }

    return false;
}

bool AddFragmentCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_project);
        if (project) {
            Fragment *fragment = new Fragment(m_fragmentName, project);
            project->addFragment(fragment);

            m_fragment = fragment->ref().light();
            return true;
        }
    }

    return false;
}

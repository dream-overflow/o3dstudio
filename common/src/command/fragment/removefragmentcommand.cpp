/**
 * @brief Remove fragment command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-15
 * @details
 */

#include <QtWidgets/QApplication>
#include <QtCore/QDataStream>

#include "o3d/studio/common/command/fragment/removefragmentcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"

using namespace o3d::studio::common;


RemoveFragmentCommand::RemoveFragmentCommand(const LightRef &fragmentRef, const LightRef &parentRef) :
    Command("o3s::common::fragment::remove", fragmentRef),
    m_parent(parentRef),
    m_fragment(fragmentRef)
{
    Q_ASSERT(m_parent.isValid());
    Q_ASSERT(m_fragment.isValid());
}

RemoveFragmentCommand::~RemoveFragmentCommand()
{
}

QString RemoveFragmentCommand::commandLabel() const
{
    return tr("Remove a fragment");
}

bool RemoveFragmentCommand::doCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level fragment, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            Fragment *fragment = project->fragment(m_fragment);
            if (fragment) {
                // backup
                QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                stream << *fragment;
                m_storedFragmentRef = fragment->ref();

                project->removeFragment(m_fragment);
                return true;
            }
        }
    }

    return false;
}

bool RemoveFragmentCommand::undoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level fragment, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            Fragment *fragment = new Fragment("", project);
            fragment->setProject(project);

            // restore content
            QDataStream stream(&m_data, QIODevice::ReadOnly);
            stream >> *fragment;
            fragment->setRef(m_storedFragmentRef);

            m_data.clear();

            project->addFragment(fragment);

            return true;
        }
    }

    return false;
}

bool RemoveFragmentCommand::redoCommand()
{
    Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        Project *project = workspace->project(m_parent);

        // first level fragment, direct to project
        if (project && m_parent.baseTypeOf(TypeRef::project())) {
            Fragment *fragment = project->fragment(m_fragment);
            if (fragment) {
                // backup
                QDataStream stream(&m_data, QIODevice::WriteOnly | QIODevice::Truncate);
                stream << *fragment;
                m_storedFragmentRef = fragment->ref();

                project->removeFragment(m_fragment);
                return true;
            }
        }
    }

    return false;
}

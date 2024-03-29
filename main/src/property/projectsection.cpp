/**
 * @brief Application project properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-08
 * @details
 */

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "projectsection.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::main;


ProjectSection::ProjectSection(const common::LightRef &ref, QWidget *parent) :
    QWidget(parent),
    m_ref(ref)
{
    m_name = fromQString(tr("undefined"));

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        const common::Project *project = workspace->project(ref);
        if (project) {
            m_name = project->name();
        }
    }

    // ui.setupUi(this);
}

ProjectSection::~ProjectSection()
{
}

o3d::String ProjectSection::name()
{
    return "o3s::workspace::project::" + m_name;
}

o3d::String ProjectSection::label()
{
    return m_label;
}

o3d::Bool ProjectSection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    return True;
}

o3d::Bool ProjectSection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();

    return True;
}

void ProjectSection::commit()
{

}

o3d::Bool ProjectSection::save()
{

    return True;
}

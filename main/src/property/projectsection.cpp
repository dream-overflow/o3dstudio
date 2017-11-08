/**
 * @brief Application project properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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


ProjectSection::ProjectSection(const QUuid &ref, QWidget *parent) :
    QWidget(parent)
{
    m_properties["uuid"] = ref;
    m_properties["name"] = tr("undefined");

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        const common::Project *project = workspace->project(ref);
        if (project) {
            m_properties["name"] = project->name();
        }
    }

    // ui.setupUi(this);
}

ProjectSection::~ProjectSection()
{
}

QString ProjectSection::name()
{
    return "o3s::workspace::project::" + m_properties["name"].toString();
}

QString ProjectSection::label()
{
    return m_properties["name"].toString();
}

bool ProjectSection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    return true;
}

bool ProjectSection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();
}

void ProjectSection::commit()
{

}

bool ProjectSection::save()
{

    return true;
}

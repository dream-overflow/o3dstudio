/**
 * @brief Application active workspace properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-08
 * @details
 */

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "workspacesection.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

using namespace o3d::studio::main;


WorkspaceSection::WorkspaceSection(const QUuid &ref, QWidget *parent) :
    QWidget(parent)
{
    m_properties["uuid"] = ref;
    m_properties["name"] = tr("undefined");

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        m_properties["name"] = workspace->name();
    }

    // ui.setupUi(this);
}

WorkspaceSection::~WorkspaceSection()
{
}

QString WorkspaceSection::name()
{
    return "o3s::workspace::" + m_properties["name"].toString();
}

QString WorkspaceSection::label()
{
    return m_properties["name"].toString();
}

bool WorkspaceSection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    return true;
}

bool WorkspaceSection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();
}

void WorkspaceSection::commit()
{

}

bool WorkspaceSection::save()
{

    return true;
}

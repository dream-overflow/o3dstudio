/**
 * @brief Application active workspace properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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


WorkspaceSection::WorkspaceSection(const Uuid &ref, QWidget *parent) :
    QWidget(parent)
{
    m_properties["uuid"] = toQString(ref.toRfc4122());
    m_properties["name"] = tr("undefined");

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        m_properties["name"] = toQString(workspace->name());
    }

    // ui.setupUi(this);
}

WorkspaceSection::~WorkspaceSection()
{
}

o3d::String WorkspaceSection::name()
{
    return "o3s::workspace::" + fromQString(m_properties["name"].toString());
}

o3d::String WorkspaceSection::label()
{
    return fromQString(m_properties["name"].toString());
}

o3d::Bool WorkspaceSection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    return True;
}

o3d::Bool WorkspaceSection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();

    return True;
}

void WorkspaceSection::commit()
{

}

o3d::Bool WorkspaceSection::save()
{

    return True;
}

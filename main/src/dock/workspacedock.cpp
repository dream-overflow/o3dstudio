/**
 * @brief Application main workspace dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#include "workspacedock.h"
#include "../mainwindow.h"

#include "o3d/studio/common/workspace/projectmodel.h"

#include <QtWidgets/QTreeView>

using namespace o3d::studio::main;


WorkspaceDock::WorkspaceDock(QWidget *parent) :
    QDockWidget(tr("Workspace"), parent),
    common::Dock()
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
}

WorkspaceDock::~WorkspaceDock()
{

}

QDockWidget *WorkspaceDock::ui()
{
    return this;
}

QString WorkspaceDock::elementName() const
{
    return("o3s::main::workspace");
}

Qt::DockWidgetArea WorkspaceDock::dockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}

void WorkspaceDock::setupUi()
{
    m_treeView = new QTreeView(this);
    setWidget(m_treeView);

    m_treeView->setModel(new common::ProjectModel());
}

/**
 * @brief New project dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#include <QtCore/QAbstractItemModel>

#include "newprojectdialog.h"

#include "common/application.h"

#include "common/workspace/workspace.h"
#include "common/workspace/workspacemanager.h"
#include "common/workspace/project.h"

#include "common/ui/uicontroller.h"
#include "common/ui/canvas/o3dcanvascontent.h"

using namespace o3d::studio::main;

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui()
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), SLOT(onButtonBox(QAbstractButton *)));
}

NewProjectDialog::~NewProjectDialog()
{

}

void NewProjectDialog::closeEvent(QCloseEvent *)
{

}

void NewProjectDialog::onButtonBox(QAbstractButton *btn)
{
    if (ui.buttonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole) {
        common::Workspace* workspace = common::Application::instance()->workspaceManager().current();
        common::Project *project = workspace->addProject("test");
        workspace->selectProject(project->uuid());

        project->initialize();

        close();
    }
}

/**
 * @brief New project dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#include <QtCore/QAbstractItemModel>

#include "newprojectdialog.h"
// #include "workspace/projectmodel.h"

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
        close();
    }
}

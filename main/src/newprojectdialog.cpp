/**
 * @brief New project dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include <QtCore/QAbstractItemModel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include "newprojectdialog.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::main;

class ProjectNameValidator : public QValidator
{
public:

    virtual State validate(QString &input, int &pos) const override;
};

QValidator::State ProjectNameValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos)

    for (int i = 0 ; i < input.length(); ++i) {
        if (!input[i].isLetterOrNumber() && input[i] != '-' && input[i] != "_") {
            input.remove(i, 1);
        }
    }

    if (input.length() < 3) {
        return Intermediate;
    }

    if (input.length() > 128) {
        return Invalid;
    }

    return Acceptable;
}

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(),
    m_valid(false)
{
    common::Settings &settings = common::Application::instance()->settings();

    ui.setupUi(this);

    // name validator
    ui.projectName->setFocus();
    ui.projectName->setValidator(new ProjectNameValidator());  // new QRegExpValidator(QRegExp("[0-9a-zA-Z_-]{3-128}"))
    ui.projectName->setPlaceholderText(tr("3 to 128 characters, only letters, digits - and _"));
    connect(ui.projectName, SIGNAL(textChanged(QString)), SLOT(onProjectNameChanged(QString)));

    // path
    QString dir = settings.get(
       "o3s::main::project::previous-folder",
       QVariant(toQString(common::Application::instance()->workspaces().defaultProjectsPath().getFullPathName()))).toString();

    ui.projectLocation->setText(dir);

    disconnect(ui.buttonBox);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), SLOT(onButtonBox(QAbstractButton *)));
    connect(ui.projectLocationSelect, SIGNAL(clicked(bool)), SLOT(onSelectProjectFolder(bool)));
}

NewProjectDialog::~NewProjectDialog()
{

}

void NewProjectDialog::accept()
{
    if (m_valid) {
        QDialog::accept();
    }
}

void NewProjectDialog::onButtonBox(QAbstractButton *btn)
{
    m_valid = false;

    QString name = ui.projectName->text();
    if (name.length() < 3) {
        ui.projectName->setStyleSheet("QLineEdit{border-color: red;}");
        return;
    }

    if (ui.buttonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole) {
        common::Workspace* workspace = common::Application::instance()->workspaces().current();
        common::Project *project = new common::Project(fromQString(name), workspace);

        project->setLocation(common::Application::instance()->workspaces().defaultProjectsPath());

        try {
            project->create();
        } catch(common::E_ProjectException &e) {
            delete project;
            QMessageBox::warning(this, tr("Project warning"), toQString(e.getMsg()));
            return;
        }

        try {
            workspace->addProject(project);
        } catch(common::E_WorkspaceException &e) {
            delete project;
            QMessageBox::warning(this, tr("Project warning"), toQString(e.getMsg()));
            return;
        }

        project->setupMasterScene();
        workspace->setActiveProject(project->ref().light());

        common::Settings &settings = common::Application::instance()->settings();
        QStringList recentsProject = settings.get("o3s::main::project::recents", QVariant(QStringList())).toStringList();
        recentsProject.append(toQString(project->path().getFullPathName()));

        if (recentsProject.size() > 10) {
            recentsProject.pop_front();
        }

        settings.set("o3s::main::project::recents", QVariant(recentsProject));

        m_valid = true;
    }
}

void NewProjectDialog::onSelectProjectFolder(bool)
{
    common::Settings &settings = common::Application::instance()->settings();

    QString dir = settings.get(
       "o3s::main::project::previous-folder",
       QVariant(toQString(common::Application::instance()->workspaces().defaultProjectsPath().getFullPathName()))).toString();

    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), dir,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        settings.set("o3s::main::project::previous-folder", dir);
        ui.projectLocation->setText(dir);
    }
}

void NewProjectDialog::onProjectNameChanged(QString)
{
    QString text = ui.projectName->text();
    int pos = text.length();
    if (ui.projectName->validator()->validate(text, pos) == QValidator::Invalid) {
        ui.projectName->setStyleSheet("QLineEdit{border-color: red;}");
    } else {
        ui.projectName->setStyleSheet("");
    }
}

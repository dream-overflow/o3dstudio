/**
 * @brief Manage workspace dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#include <QtCore/QAbstractItemModel>

#include "manageworkspacesdialog.h"
// #include "workspace/workspacemodel.h"

using namespace o3d::studio::main;

ManageWorkspaceDialog::ManageWorkspaceDialog(QWidget *parent) :
    QDialog(parent),
    ui()
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), SLOT(onButtonBox(QAbstractButton *)));

    setupWorkspaces();
}

ManageWorkspaceDialog::~ManageWorkspaceDialog()
{

}

void ManageWorkspaceDialog::closeEvent(QCloseEvent *)
{

}
/*
void ManageWorkspaceDialog::onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) {
        PropertySection *previousPs = static_cast<PropertyItem*>(previous.internalPointer())->propertySection();
        previousPs->commit();

        ui.propertyLabel->setText("");
    }

    if (current.isValid()) {
        PropertyItem *item = static_cast<PropertyItem*>(current.internalPointer());
        PropertySection *currentPs = item->propertySection();

        if (!ui.propertyFrame->layout()) {
            ui.propertyFrame->setLayout(new QHBoxLayout());
        } else {
            QLayoutItem *child;
            while ((child = ui.propertyFrame->layout()->takeAt(0)) != 0) {
                if (child->widget()) {
                    child->widget()->setParent(nullptr);
                }
                delete child;
            }
        }

        currentPs->setupSection(ui.propertyFrame);

        // compound label
        QString label = currentPs->label();

        PropertyItem *parent = item->parentItem();
        while (parent != nullptr && parent->propertySection()) {
            label.append(" > ").append(parent->propertySection()->label());
            parent = parent->parentItem();
        }

        ui.propertyLabel->setText(label);
    }
}

QModelIndex recursiveSave(QAbstractItemModel *model, const QModelIndex &parent) {
    if (!parent.isValid()) {
        return parent;
    }

    PropertyItem *item = static_cast<PropertyItem*>(parent.internalPointer());
    PropertySection *currentPs = item->propertySection();

    if (currentPs) {
        currentPs->save();
    }

    int count = model->rowCount(parent);
    for(int i = 0; i < count; ++i) {
        QModelIndex current = model->index(i, 0, parent);

        if (current.isValid()) {
            recursiveSave(model, current);
        }
    }

    return parent;
}*/

void ManageWorkspaceDialog::onButtonBox(QAbstractButton *btn)
{
    if (ui.buttonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole) {
        close();
    }
}

void ManageWorkspaceDialog::setupWorkspaces()
{
   /* QList<PropertySection*> sections;

    // setup standard static sections
    sections.append(new SectionNode("o3s::main", tr("Global")));
    sections.append(new DisplaySection());

    sections.append(new SectionNode("o3s::workspace", tr("Workspace")));
    sections.append(new SectionNode("o3s::workspace::project", tr("Projects")));
    sections.append(new SectionNode("o3s::plugin", tr("Plugins")));
    sections.append(new SectionNode("o3s::tool", tr("Tools")));

    PropertyModel *model = new PropertyModel(sections, this);
    ui.categoryTree->setModel(model);
    ui.categoryTree->setColumnWidth(0, 200);
    ui.categoryTree->setColumnWidth(1, 50);

    QItemSelectionModel *selectionModel = ui.categoryTree->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSectionPropertyChanged(const QModelIndex &, const QModelIndex &)));
*/
}

/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QAbstractItemModel>

#include "preferencesdialog.h"

#include "common/property/propertymodel.h"
#include "common/property/sectionnode.h"

#include "property/displaysection.h"

using namespace o3d::studio::main;

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui()
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton *)), SLOT(onButtonBox(QAbstractButton *)));

    setupCategories();
}

PreferencesDialog::~PreferencesDialog()
{

}

void PreferencesDialog::closeEvent(QCloseEvent *)
{

}

void PreferencesDialog::onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (previous.isValid()) {
        common::PropertySection *previousPs = static_cast<common::PropertyItem*>(previous.internalPointer())->propertySection();
        previousPs->commit();

        ui.propertyLabel->setText("");
    }

    if (current.isValid()) {
        common::PropertyItem *item = static_cast<common::PropertyItem*>(current.internalPointer());
        common::PropertySection *currentPs = item->propertySection();

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

        common::PropertyItem *parent = item->parentItem();
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

    using o3d::studio::common::PropertyItem;
    using o3d::studio::common::PropertySection;

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
}

void PreferencesDialog::onButtonBox(QAbstractButton *btn)
{
    if (ui.buttonBox->buttonRole(btn) == QDialogButtonBox::ApplyRole ||
        ui.buttonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole) {

        // commit current
        QModelIndex current = ui.categoryTree->currentIndex();
        if (current.isValid()) {
            common::PropertyItem *item = static_cast<common::PropertyItem*>(current.internalPointer());
            common::PropertySection *currentPs = item->propertySection();

            currentPs->commit();
        }

        // recursive save
        QModelIndex parent = ui.categoryTree->model()->index(0, 0);
        recursiveSave(ui.categoryTree->model(), parent);
    }

    if (ui.buttonBox->buttonRole(btn) == QDialogButtonBox::AcceptRole) {
        close();
    }
}

void PreferencesDialog::setupCategories()
{
    QList<common::PropertySection*> sections;

    // setup standard static sections
    sections.append(new common::SectionNode("o3s::main", tr("Global")));
    sections.append(new DisplaySection());

    sections.append(new common::SectionNode("o3s::workspace", tr("Workspace")));
    sections.append(new common::SectionNode("o3s::workspace::project", tr("Projects")));
    sections.append(new common::SectionNode("o3s::plugin", tr("Plugins")));
    sections.append(new common::SectionNode("o3s::tool", tr("Tools")));

    common::PropertyModel *model = new common::PropertyModel(sections, this);
    ui.categoryTree->setModel(model);
    ui.categoryTree->setColumnWidth(0, 200);
    ui.categoryTree->setColumnWidth(1, 50);

    QItemSelectionModel *selectionModel = ui.categoryTree->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSectionPropertyChanged(const QModelIndex &, const QModelIndex &)));
}

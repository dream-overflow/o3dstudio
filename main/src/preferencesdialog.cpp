/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QAbstractItemModel>
#include <QtWidgets/QApplication>

#include "preferencesdialog.h"

#include "common/property/propertyitem.h"
#include "common/property/propertymodel.h"
#include "common/property/propertysectionnode.h"

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
    QModelIndex current = ui.categoryTree->currentIndex();

    // cleanup
    onSectionPropertyChanged(QModelIndex(), current);
}

void PreferencesDialog::closeEvent(QCloseEvent *)
{

}

void PreferencesDialog::changeEvent(QEvent *event)
{
    if (event != nullptr) {
        switch(event->type()) {
            // this event is send if a translator is loaded
            case QEvent::LanguageChange:
            {
                QModelIndex current = ui.categoryTree->currentIndex();

                // cleanup
                onSectionPropertyChanged(QModelIndex(), current);

                common::PropertyItem *propertyItem = static_cast<common::PropertyItem*>(current.internalPointer());
                int row = current.row();
                int column = current.column();
                QString path = propertyItem->path();
                propertyItem = nullptr;

                // renegerate model for translations
                setupCategories();

                // reload
                QModelIndex rootIndex = ui.categoryTree->model()->index(0, 0);
                common::PropertyItem *rootItem = static_cast<common::PropertyItem*>(rootIndex.internalPointer());

                propertyItem = rootItem->find(path);
                QModelIndex parentIndex = rootIndex.sibling(row, column);

                current = ui.categoryTree->model()->index(propertyItem->row(), 0, parentIndex);
                ui.categoryTree->setCurrentIndex(current);

                QItemSelectionModel *selection = ui.categoryTree->selectionModel();
                selection->select(current, QItemSelectionModel::Select);
            }
                break;

            default:
                break;
        }
    }
    QDialog::changeEvent(event);
}

void PreferencesDialog::onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (!ui.propertyFrame->layout()) {
        ui.propertyFrame->setLayout(new QHBoxLayout());
    }

    if (previous.isValid()) {
        common::PropertySection *previousPs = static_cast<common::PropertyItem*>(previous.internalPointer())->section();
        previousPs->commit();
        previousPs->cleanupSection(ui.propertyFrame);

        ui.propertyLabel->setText("");
    }

    if (current.isValid()) {
        common::PropertyItem *item = static_cast<common::PropertyItem*>(current.internalPointer());
        common::PropertySection *currentPs = item->section();

        currentPs->setupSection(ui.propertyFrame);

        // compound label
        QString label = currentPs->label();

        common::PropertyItem *parent = item->parentItem();
        while (parent != nullptr && parent->section()) {
            QString l = parent->section()->label();
            label.append(" > ").append(l);
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
    PropertySection *currentPs = item->section();

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
            common::PropertySection *currentPs = item->section();

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
    common::PropertyModel *model = static_cast<common::PropertyModel*>(ui.categoryTree->model());

    if (model) {
        ui.categoryTree->setModel(nullptr);
        delete model;
    }

    QList<common::PropertySection*> sections;

    // setup standard static sections
    sections.append(new common::PropertySectionNode("o3s::main", tr("General")));
    sections.append(new DisplaySection());

    sections.append(new common::PropertySectionNode("o3s::workspace", tr("Workspace")));
    sections.append(new common::PropertySectionNode("o3s::workspace::project", tr("Projects")));
    sections.append(new common::PropertySectionNode("o3s::plugin", tr("Plugins")));
    sections.append(new common::PropertySectionNode("o3s::tool", tr("Tools")));

    model = new common::PropertyModel(sections, this);
    ui.categoryTree->setModel(model);
    ui.categoryTree->setColumnWidth(0, 200);
    ui.categoryTree->setColumnWidth(1, 50);

    QItemSelectionModel *selectionModel = ui.categoryTree->selectionModel();
    connect(selectionModel,
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(onSectionPropertyChanged(const QModelIndex &, const QModelIndex &)));
}

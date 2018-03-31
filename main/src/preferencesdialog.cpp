/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <algorithm>

#include <QtCore/QAbstractItemModel>
#include <QtWidgets/QApplication>

#include "preferencesdialog.h"

#include "o3d/studio/common/property/propertyitem.h"
#include "o3d/studio/common/property/propertymodel.h"
#include "o3d/studio/common/property/propertysectionnode.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

#include "property/displaysection.h"
#include "property/workspacesection.h"
#include "property/projectsection.h"

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
                String path = propertyItem->path();
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
        QString label = toQString(currentPs->label());

        common::PropertyItem *parent = item->parentItem();
        while (parent != nullptr && parent->section()) {
            QString l = toQString(parent->section()->label());
            label.prepend(" > ").prepend(l);
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

bool caseInsensitiveLessThan(const o3d::studio::common::Project *p1, const o3d::studio::common::Project *p2)
{
    return p1->name().toLower() < p2->name().toLower();
}

void PreferencesDialog::setupCategories()
{
    common::PropertyModel *model = static_cast<common::PropertyModel*>(ui.categoryTree->model());

    if (model) {
        ui.categoryTree->setModel(nullptr);
        delete model;
    }

    std::list<common::PropertySection*> sections;

    // setup standard static sections
    sections.push_back(new common::PropertySectionNode("o3s::main", fromQString(tr("General"))));
    sections.push_back(new DisplaySection());

    sections.push_back(new common::PropertySectionNode("o3s::workspace", fromQString(tr("Workspace"))));
    sections.push_back(new common::PropertySectionNode("o3s::workspace::project", fromQString(tr("Projects"))));
    sections.push_back(new common::PropertySectionNode("o3s::plugin", fromQString(tr("Plugins"))));
    sections.push_back(new common::PropertySectionNode("o3s::tool", fromQString(tr("Tools"))));

    // workspace
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        // specialized workspace section node
        sections.push_back(new WorkspaceSection(workspace->uuid()));
        // @todo others workspace section

        std::list<common::Project*> loadedProjectList = workspace->loadedProjectList();

        // sort by project name
        loadedProjectList.sort(caseInsensitiveLessThan);
        // std::sort(loadedProjectList.begin(), loadedProjectList.end(), caseInsensitiveLessThan);

        for (common::Project *project : loadedProjectList) {
            // specialized project section node
            sections.push_back(new ProjectSection(project->ref().light()));
            // @todo project sub-sections
        }
    }

    model = new common::PropertyModel(sections, this);
    ui.categoryTree->setModel(model);
    ui.categoryTree->setColumnWidth(0, 200);
    ui.categoryTree->setColumnWidth(1, 50);

    QItemSelectionModel *selectionModel = ui.categoryTree->selectionModel();
    connect(selectionModel,
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(onSectionPropertyChanged(const QModelIndex &, const QModelIndex &)));
}

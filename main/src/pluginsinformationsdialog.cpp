/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QAbstractItemModel>

#include "pluginsinformationsdialog.h"

#include "o3d/studio/common/plugin/pluginmodel.h"
#include "o3d/studio/common/plugin/pluginsectionnode.h"

#include "o3d/studio/common/modulemanager.h"
#include "o3d/studio/common/module.h"

using namespace o3d::studio::main;

PluginsInformationsDialog::PluginsInformationsDialog(QWidget *parent) :
    QDialog(parent),
    ui()
{
    ui.setupUi(this);

    connect(ui.pluginDetailsButton, SIGNAL(clicked()), SLOT(onPluginDetailsClicked()));
    connect(ui.pluginErrorsButton, SIGNAL(clicked()), SLOT(onPluginErrorDetailsClicked()));

    setupCategories();
}

PluginsInformationsDialog::~PluginsInformationsDialog()
{

}

void PluginsInformationsDialog::closeEvent(QCloseEvent *)
{

}

void PluginsInformationsDialog::changeEvent(QEvent *event)
{
    if (event != nullptr) {
        switch(event->type()) {
            // this event is send if a translator is loaded
            case QEvent::LanguageChange:
                ui.retranslateUi(this);
                break;

            default:
                break;
        }
    }
    QDialog::changeEvent(event);
}

void PluginsInformationsDialog::onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous)
{
/*    if (previous.isValid()) {
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
    }*/
}

void PluginsInformationsDialog::onPluginDetails(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    if (index.internalPointer() == nullptr) {
        return;
    }

    common::ModuleManager *moduleManager = common::ModuleManager::instance();
    common::PluginItem *pluginItem = static_cast<common::PluginItem*>(index.internalPointer());

    const common::Module* module = moduleManager->module(fromQString(pluginItem->name()));
    if (module == nullptr) {
        return;
    }

    const common::ModuleInfo info = module->provideInfo();
    showDetailsDialog(module->name(), info.properties());
}

void PluginsInformationsDialog::onPluginErrorDetailsClicked()
{
    QModelIndex index = ui.pluginsTree->currentIndex();

    if (!index.isValid()) {
        return;
    }

    if (index.internalPointer() == nullptr) {
        return;
    }

    common::ModuleManager *moduleManager = common::ModuleManager::instance();
    common::PluginItem *pluginItem = static_cast<common::PluginItem*>(index.internalPointer());

    const common::Module* module = moduleManager->module(fromQString(pluginItem->name()));
    if (module == nullptr) {
        return;
    }

    const T_StringList &report = module->report();
    if (report.size() > 0) {
        // @todo open error details dialog
    }
}

void PluginsInformationsDialog::onPluginDetailsClicked()
{
    QModelIndex index = ui.pluginsTree->currentIndex();

    if (!index.isValid()) {
        return;
    }

    if (index.internalPointer() == nullptr) {
        return;
    }

    common::ModuleManager *moduleManager = common::ModuleManager::instance();
    common::PluginItem *pluginItem = static_cast<common::PluginItem*>(index.internalPointer());

    const common::Module* module = moduleManager->module(fromQString(pluginItem->name()));
    if (module == nullptr) {
        return;
    }

    const common::ModuleInfo info = module->provideInfo();
    showDetailsDialog(module->name(), info.properties());
}
/*
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

static QString makeParentPath(const QStringList &path, int minus) {
    QString parentPath = "";
    for (int i = 0; i < path.length() - minus; ++i) {
        if (parentPath.length() > 0) {
            parentPath.append("::");
        }

        parentPath.append(path.at(i));
    }

    return parentPath;
}

void PluginsInformationsDialog::setupCategories()
{
    QList<common::PluginSection*> sections;

    // setup standard static sections
    sections.append(new common::PluginSectionNode("o3s::plugin::o3dstudio", tr("Objective-3D Studio")));
    sections.append(new common::PluginSectionNode("o3s::plugin::tool", tr("Externals tools")));
    sections.append(new common::PluginSectionNode("o3s::plugin::importer", tr("Importers")));
    sections.append(new common::PluginSectionNode("o3s::plugin::exporter", tr("Exporters")));
    sections.append(new common::PluginSectionNode("o3s::plugin::builder", tr("Builders")));

    common::PluginModel *model = new common::PluginModel(sections, this);
    ui.pluginsTree->setModel(model);
    ui.pluginsTree->setColumnWidth(0, 200);
    ui.pluginsTree->setColumnWidth(1, 50);
    ui.pluginsTree->setColumnWidth(2, 70);
    ui.pluginsTree->setColumnWidth(3, 200);
    ui.pluginsTree->setColumnWidth(4, 50);

    common::ModuleManager *moduleManager = common::ModuleManager::instance();

    // add loaded modules
    QString moduleName = "";
    common::PluginItem *itemParent = nullptr;
    common::PluginItem *rootItem = static_cast<common::PluginItem*>(model->index(0, 0, QModelIndex()).internalPointer());

    for (String moduleName : moduleManager->moduleList()) {
        const common::Module* module = moduleManager->module(moduleName);
        const common::ModuleInfo info = module->provideInfo();

        QList<QVariant> data;
        data << toQString(info.verbose()) << module->started() << toQString(info.version()) << toQString(info.vendor());

        itemParent = rootItem->find(toQString(info.section()));
        if (itemParent == nullptr) {
            // @todo generic or create
        }

        common::PluginItem *pluginModel = new common::PluginItem(toQString(info.section()), toQString(moduleName), data, itemParent);
        itemParent->appendChild(pluginModel);
    }

    QItemSelectionModel *selectionModel = ui.pluginsTree->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(onSectionPropertyChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui.pluginsTree, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(onPluginDetails(const QModelIndex &)));
}

void PluginsInformationsDialog::showDetailsDialog(const String &name, std::map<String, String> properties)
{
    // @todo
}

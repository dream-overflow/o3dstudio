/**
 * @brief Application project model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/projectitem.h"

#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/asset.h"

#include "o3d/studio/common/ui/uiutils.h"

using namespace o3d::studio::common;

ProjectModel::ProjectModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new ProjectItem(nullptr, LightRef(), "o3s::workspace", QIcon());
}

ProjectModel::~ProjectModel()
{
    if (m_rootItem) {
        delete m_rootItem;
    }
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    ProjectItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());
    }

    ProjectItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex ProjectModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    ProjectItem *childItem = static_cast<ProjectItem*>(child.internalPointer());
    ProjectItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    ProjectItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<ProjectItem*>(parent.internalPointer())->columnCount();
    } else {
        return m_rootItem->columnCount();
    }
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    ProjectItem *item = static_cast<ProjectItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    } else if (role == Qt::DecorationRole) {
        return item->decoration(index.column());
    }
/*
    if (role == Qt::DisplayRole && index.column() != 1) {
        return item->data(index.column());
    } else if (role == Qt::CheckStateRole && index.column() == 1) {
        int rows = item->childCount();
        if (rows > 0 ) {
            int started = 0;
            for (int i = 0; i < rows; ++i) {
                if (item->child(i)->data(1).toBool()) {
                    ++started;
                }
            }
            if (started == 0) {
                return Qt::Unchecked;
            } else if (started == rows) {
                return Qt::Checked;
            }

            return Qt::PartiallyChecked;
        } else {
            return item->data(index.column()).toBool() ? Qt::Checked : Qt::Unchecked;
        }
    } else {*/
        return QVariant();
    //}
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)

    // no header
    return QVariant();
}

const ProjectItem *ProjectModel::find(const LightRef &ref) const
{
    return m_rootItem->find(ref);
}

ProjectItem *ProjectModel::find(const LightRef &ref)
{
    return m_rootItem->find(ref);
}

ProjectItem *ProjectModel::addProject(Project *project)
{
    if (!project) {
        return nullptr;
    }

    int n = m_rootItem->childCount();

    beginInsertRows(QModelIndex(), n, n);

    ProjectItem *item = new ProjectItem(project,
                                        project->ref().light(),
                                        project->name(),
                                        QIcon::fromTheme("document-open"),
                                        m_rootItem);
    m_rootItem->appendChild(item);

    // and add the initial structure of hubs
    {
        std::list<Hub*> hubs = project->hubs(true);
        for (Hub *hub : hubs) {
            addHub(hub);
        }
    }

    // ...fragments
    {
        std::list<Fragment*> fragments = project->fragments();
        for (Fragment *fragment : fragments) {
            addFragment(fragment);
        }
    }

    // ...assets
    {
        std::list<Asset*> assets = project->assets();
        for (Asset *asset : assets) {
            addAsset(asset);
        }
    }

    endInsertRows();

    return item;
}

void ProjectModel::removeProject(const common::LightRef &ref)
{
    if (!ref.isValid()) {
        return;
    }

    // find the project
    ProjectItem *projectItem = m_rootItem->find(ref);
    if (!projectItem) {
        return;
    }

    int n = projectItem->row();

    beginRemoveRows(QModelIndex(), n, n);
    m_rootItem->removeChild(n);

    delete projectItem;

    endRemoveRows();
}

ProjectItem *ProjectModel::addHub(common::Hub *hub)
{
    if (!hub) {
        return nullptr;
    }

    ProjectItem *parentItem = find(hub->parent()->ref().light());

    if (!parentItem) {
        return nullptr;
    }

    int n = parentItem->childCount();

    QModelIndex parentIndex = createIndex(parentItem->row(), 0, parentItem);

    beginInsertRows(parentIndex, n, n);

    ProjectItem *item = new ProjectItem(hub,
                                        hub->ref().light(),
                                        hub->name(),
                                        UiUtils::tintIcon(":/icons/device_hub_black.svg"),
                                        parentItem);
    parentItem->appendChild(item);

//    // iterator over children
//    common::Hub *node = nullptr;
//    foreach (node, hub->hubs(true)) {
//        addHub(node);
//    }
    endInsertRows();

    return item;
}

void ProjectModel::removeHub(const common::LightRef &ref)
{
    if (!ref.isValid()) {
        return;
    }

    ProjectItem *hubItem = find(ref);
    if (!hubItem) {
        return;
    }

    // find the parent
    ProjectItem *parentItem = hubItem->parentItem();
    if (!parentItem) {
        return;
    }

    QModelIndex parentIndex = createIndex(parentItem->row(), 0, parentItem);

    int n = hubItem->row();

    beginRemoveRows(parentIndex, n, n);
    parentItem->removeChild(n);

    delete hubItem;

    endRemoveRows();
}

ProjectItem *ProjectModel::addFragment(common::Fragment *fragment)
{
    if (!fragment) {
        return nullptr;
    }

    ProjectItem *parentItem = find(fragment->parent()->ref().light());

    if (!parentItem) {
        return nullptr;
    }

    int n = parentItem->childCount();

    QModelIndex parentIndex = index(parentItem->row(), 0);  // @todo for n*subs fragments

    beginInsertRows(parentIndex, n, n);

    ProjectItem *item = new ProjectItem(fragment,
                                        fragment->ref().light(),
                                        fragment->name(),
                                        UiUtils::tintIcon(":/icons/fragment_flat.svg"),
                                        parentItem);
    parentItem->appendChild(item);

    endInsertRows();

    return item;
}

void ProjectModel::removeFragment(const common::LightRef &ref)
{
    if (!ref.isValid()) {
        return;
    }

    ProjectItem *fragmentItem = find(ref);
    if (!fragmentItem) {
        return;
    }

    // find the parent
    ProjectItem *parentItem = fragmentItem->parentItem();
    if (!parentItem) {
        return;
    }

    QModelIndex parentIndex = index(parentItem->row(), 0);  // @todo for n*subs fragments

    int n = fragmentItem->row();

    beginRemoveRows(parentIndex, n, n);
    parentItem->removeChild(n);

    delete fragmentItem;

    endRemoveRows();
}

ProjectItem *ProjectModel::addAsset(common::Asset *asset)
{
    if (!asset) {
        return nullptr;
    }

    ProjectItem *parentItem = find(asset->parent()->ref().light());

    if (!parentItem) {
        return nullptr;
    }

    int n = parentItem->childCount();

    QModelIndex parentIndex = index(parentItem->row(), 0);  // @todo for n*subs asset

    beginInsertRows(parentIndex, n, n);

    ProjectItem *item = new ProjectItem(asset,
                                        asset->ref().light(),
                                        asset->name(),
                                        UiUtils::tintIcon(":/icons/fragment_flat.svg"),
                                        parentItem);
    parentItem->appendChild(item);

    endInsertRows();

    return item;
}

void ProjectModel::removeAsset(const common::LightRef &ref)
{
    if (!ref.isValid()) {
        return;
    }

    ProjectItem *assetItem = find(ref);
    if (!assetItem) {
        return;
    }

    // find the parent
    ProjectItem *parentItem = assetItem->parentItem();
    if (!parentItem) {
        return;
    }

    QModelIndex parentIndex = index(parentItem->row(), 0);  // @todo for n*subs assets

    int n = assetItem->row();

    beginRemoveRows(parentIndex, n, n);
    parentItem->removeChild(n);

    delete assetItem;

    endRemoveRows();
}

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
/*
ProjectItem* createItem(const QString &section, const QString &name, const QString &label, ProjectItem *itemParent)
{
    QList<QVariant> itemData;
    itemData << label << QString() << QString();

    ProjectItem *item = new ProjectItem(section, name, itemData, itemParent);
    return item;
}*/
/*
void ProjectModel::setupModelData(const QList<PluginSection *> &data, ProjectItem *parent)
{
    PluginItem *rootItem = m_rootItem;

    PluginSection *section = nullptr;
    PluginItem *itemParent = nullptr;

    foreach (section, data) {
        QStringList path = section->name().split("::");
        QString leaf = path.last();

        QString parentPath = makeParentPath(path, 1);
        itemParent = rootItem->find(parentPath);

        // no parent found create intermediates nodes
        if (itemParent == nullptr) {
            // start at root, and test for each sub-level
            itemParent = m_rootItem;

            for (int i = 0; i < path.length() - 1; ++i) {
                QString lParentPath = makeParentPath(path, path.length() - i - 1);

                // is level exists
                PluginItem *lParent = rootItem->find(lParentPath);

                // create it from current itemParent
                if (lParent == nullptr) {
                    lParent = createItem(path[i], path[i], path[i], itemParent);
                    itemParent->appendChild(lParent);
                }

                itemParent = lParent;
            }
        }

        PluginItem *item = createItem(leaf, leaf, section->label(), itemParent);
        itemParent->appendChild(item);
    }
}
*/

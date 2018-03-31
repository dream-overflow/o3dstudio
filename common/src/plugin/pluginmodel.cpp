/**
 * @brief Application plugin model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-28
 * @details
 */

#include "o3d/studio/common/plugin/pluginmodel.h"
#include "o3d/studio/common/plugin/pluginitem.h"
#include "o3d/studio/common/plugin/pluginsection.h"

using namespace o3d::studio::common;

PluginModel::PluginModel(const QList<PluginSection*> &data, QObject *parent) :
    QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << tr("Name") << tr("Active") << tr("Version") << tr("Vendor");  // << tr("Count");
    m_rootItem = new PluginItem("o3s::plugin", "", rootData);

    setupModelData(data, m_rootItem);
}

PluginModel::~PluginModel()
{
    if (m_rootItem) {
        delete m_rootItem;
    }
}

QModelIndex PluginModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    PluginItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<PluginItem*>(parent.internalPointer());
    }

    PluginItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex PluginModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    PluginItem *childItem = static_cast<PluginItem*>(child.internalPointer());
    PluginItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int PluginModel::rowCount(const QModelIndex &parent) const
{
    PluginItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<PluginItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int PluginModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<PluginItem*>(parent.internalPointer())->columnCount();
    } else {
        return m_rootItem->columnCount();
    }
}

QVariant PluginModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    PluginItem *item = static_cast<PluginItem*>(index.internalPointer());

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
    } else {
        return QVariant();
    }
}

Qt::ItemFlags PluginModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant PluginModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }

    return QVariant();
}

PluginItem *PluginModel::rootItem()
{
    return m_rootItem;
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

PluginItem* createItem(const QString &section, const QString &name, const QString &label, PluginItem *itemParent)
{
    QList<QVariant> itemData;
    itemData << label << QString() << QString();

    PluginItem *item = new PluginItem(section, name, itemData, itemParent);
    return item;
}

void PluginModel::setupModelData(const QList<PluginSection *> &data, PluginItem *parent)
{
    Q_UNUSED(parent)

    PluginItem *rootItem = m_rootItem;
    PluginItem *itemParent = nullptr;

    for (PluginSection *section : data) {
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

/**
 * @brief Application property model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#include "o3d/studio/common/property/propertymodel.h"
#include "o3d/studio/common/property/propertyitem.h"
#include "o3d/studio/common/property/propertysection.h"

using namespace o3d::studio::common;

PropertyModel::PropertyModel(const QList<PropertySection*> &data, QObject *parent) :
    QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << tr("Properties");
    m_rootItem = new PropertyItem("o3s", rootData);

    setupModelData(data, m_rootItem);
}

PropertyModel::~PropertyModel()
{
    if (m_rootItem) {
        delete m_rootItem;
    }

    PropertySection *section;
    foreach (section, m_sections) {
        delete section;
    }
}

QModelIndex PropertyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    PropertyItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<PropertyItem*>(parent.internalPointer());
    }

    PropertyItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex PropertyModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    PropertyItem *childItem = static_cast<PropertyItem*>(child.internalPointer());
    PropertyItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int PropertyModel::rowCount(const QModelIndex &parent) const
{
    PropertyItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<PropertyItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int PropertyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<PropertyItem*>(parent.internalPointer())->columnCount();
    } else {
        return m_rootItem->columnCount();
    }
}

QVariant PropertyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    PropertyItem *item = static_cast<PropertyItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant PropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }

    return QVariant();
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

PropertyItem* createItem(const QString &section, const QString &label, PropertySection *ps, PropertyItem *itemParent)
{
    QList<QVariant> itemData;
    itemData << label;

    PropertyItem *item = new PropertyItem(section, itemData, ps, itemParent);
    return item;
}

void PropertyModel::setupModelData(const QList<PropertySection *> &data, PropertyItem *parent)
{
    Q_UNUSED(parent)

    PropertyItem *rootItem = m_rootItem;

    PropertySection *propertySection = nullptr;
    PropertyItem *itemParent = nullptr;

    foreach (propertySection, data) {
        QStringList path = propertySection->name().split("::");
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
                PropertyItem *lParent = rootItem->find(lParentPath);

                // create it from current itemParent
                if (lParent == nullptr) {
                    lParent = createItem(path[i], path[i], nullptr, itemParent);
                    itemParent->appendChild(lParent);
                }

                itemParent = lParent;
            }
        }

        PropertyItem *item = createItem(leaf, propertySection->label(), propertySection, itemParent);
        itemParent->appendChild(item);

        if (m_sections.indexOf(propertySection) < 0) {
            m_sections.append(propertySection);
        }
    }
}

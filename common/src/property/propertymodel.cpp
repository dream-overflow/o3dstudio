/**
 * @brief Application property model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#include <QtCore/QCoreApplication>

#include "o3d/studio/common/property/propertymodel.h"
#include "o3d/studio/common/property/propertyitem.h"
#include "o3d/studio/common/property/propertysection.h"

#include <algorithm>

#include <o3d/core/stringlist.h>

using namespace o3d::studio::common;

PropertyModel::PropertyModel(const std::list<PropertySection *> &data, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new PropertyItem("o3s", fromQString(tr("Properties")));

    setupModelData(data, m_rootItem);
}

PropertyModel::~PropertyModel()
{
    if (m_rootItem) {
        delete m_rootItem;
    }

    for (PropertySection *section : m_sections) {
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

PropertyItem* createItem(const o3d::String &section, const o3d::String &label, PropertySection *ps, PropertyItem *itemParent)
{
    PropertyItem *item = new PropertyItem(section, label, ps, itemParent);
    return item;
}

void PropertyModel::setupModelData(const std::list<PropertySection *> &data, PropertyItem *parent)
{
    Q_UNUSED(parent)

    PropertyItem *rootItem = m_rootItem;
    PropertyItem *itemParent = nullptr;

    for (PropertySection *propertySection : data) {
        QStringList path = toQString(propertySection->name()).split("::");
        QString leaf = path.last();

        QString parentPath = makeParentPath(path, 1);
        itemParent = rootItem->find(fromQString(parentPath));

        // no parent found create intermediates nodes
        if (itemParent == nullptr) {
            // start at root, and test for each sub-level
            itemParent = m_rootItem;

            for (int i = 0; i < path.length() - 1; ++i) {
                QString lParentPath = makeParentPath(path, path.length() - i - 1);

                // is level exists
                PropertyItem *lParent = rootItem->find(fromQString(lParentPath));

                // create it from current itemParent
                if (lParent == nullptr) {
                    lParent = createItem(fromQString(path[i]), fromQString(path[i]), nullptr, itemParent);
                    itemParent->appendChild(lParent);
                }

                itemParent = lParent;
            }
        }

        PropertyItem *item = createItem(fromQString(leaf), propertySection->label(), propertySection, itemParent);
        itemParent->appendChild(item);

        auto it = std::find(m_sections.begin(), m_sections.end(), propertySection);
        if (it == m_sections.end()) {
            m_sections.push_back(propertySection);
        }
    }
}

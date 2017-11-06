/**
 * @brief Application property item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#include "o3d/studio/common/property/propertyitem.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


PropertyItem::PropertyItem(const QString section, const QList<QVariant> &data, PropertySection *ps, PropertyItem *parentItem) :
    m_path(""),
    m_itemData(data),
    m_parentItem(parentItem),
    m_section(ps)
{
    if (parentItem) {
        m_path = parentItem->m_path + "::" + section;
    } else {
        m_path = section;
    }
}

PropertyItem::~PropertyItem()
{
    qDeleteAll(m_childItems);
}

void PropertyItem::appendChild(PropertyItem *item)
{
    m_childItems.append(item);
}

PropertyItem *PropertyItem::child(int row)
{
    return m_childItems.value(row);
}

int PropertyItem::childCount() const
{
    return m_childItems.count();
}

int PropertyItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<PropertyItem*>(this));
    }

    return 0;
}

int PropertyItem::columnCount() const
{
    return m_itemData.count() + 1;
}

QVariant PropertyItem::data(int column) const
{
    if (column == 1) {
        if (m_parentItem == nullptr) {
            return QVariant(QObject::tr("Count"));
        } else {
            return childCount();
        }
    }

    return m_itemData.value(column);
}

PropertyItem* PropertyItem::parentItem()
{
    return m_parentItem;
}

PropertyItem *PropertyItem::find(const QString &path)
{
    if (path == m_path) {
        return this;
    }

    QStringList dpath = path.split("::");
    QStringList lpath = m_path.split("::");

    PropertyItem *result = nullptr;

    if (lpath.length() < dpath.length()) {
        PropertyItem *child = nullptr;
        foreach (child, m_childItems) {
            result = child->find(path);

            if (result != nullptr) {
                return result;
            }
        }
    }

    return nullptr;
}

PropertySection *PropertyItem::section()
{
    return m_section;
}

const PropertySection *PropertyItem::section() const
{
    return m_section;
}

const QString &PropertyItem::path() const
{
    return m_path;
}

/**
 * @brief Application property item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#include "o3d/studio/common/property/propertyitem.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


PropertyItem::PropertyItem(const String section, const String &label, PropertySection *ps, PropertyItem *parentItem) :
    m_path(""),
    m_label(label),
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
    m_childItems.push_back(item);
}

PropertyItem *PropertyItem::child(int row)
{
    int i = 0;
    auto it = m_childItems.begin();
    while (i < row) {
        ++it;
        ++i;

        if (it == m_childItems.end()) {
            return nullptr;
        }
    }

    return *it;
}

int PropertyItem::childCount() const
{
    return m_childItems.size();
}

int PropertyItem::row() const
{
    if (m_parentItem) {
        int n = 0;
        for (const PropertyItem* item : m_parentItem->m_childItems) {
            if (item == this) {
                return n;
            }
            ++n;
        }
    }

    return 0;
}

int PropertyItem::columnCount() const
{
    return 2;
}

QVariant PropertyItem::data(int column) const
{
    if (column == 0) {
        return toQString(m_label);
    } else if (column == 1) {
        if (m_parentItem == nullptr) {
            return QVariant(QObject::tr("Count"));
        } else {
            return childCount();
        }
    }

    return QVariant();
}

PropertyItem* PropertyItem::parentItem()
{
    return m_parentItem;
}

PropertyItem *PropertyItem::find(const String &path)
{
    if (path == m_path) {
        return this;
    }

    QStringList dpath = toQString(path).split("::");
    QStringList lpath = toQString(m_path).split("::");

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

const o3d::String &PropertyItem::path() const
{
    return m_path;
}

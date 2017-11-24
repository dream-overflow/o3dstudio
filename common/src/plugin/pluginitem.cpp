/**
 * @brief Application plugin item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-28
 * @details
 */

#include "o3d/studio/common/plugin/pluginitem.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


PluginItem::PluginItem(const QString &section, const QString &name, const QList<QVariant> &data, PluginItem *parentItem) :
    m_path(""),
    m_name(name),
    m_itemData(data),
    m_parentItem(parentItem)
{
    if (parentItem) {
        m_path = parentItem->m_path + "::" + section;
    } else {
        m_path = section;
    }
}

PluginItem::~PluginItem()
{
    for (PluginItem *item : m_childItems) {
        deletePtr(item);
    }

    m_childItems.clear();
}

void PluginItem::appendChild(PluginItem *item)
{
    m_childItems.append(item);
}

PluginItem *PluginItem::child(int row)
{
    return m_childItems.value(row);
}

int PluginItem::childCount() const
{
    return m_childItems.count();
}

int PluginItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<PluginItem*>(this));
    }

    return 0;
}

int PluginItem::columnCount() const
{
    return m_itemData.count() + 1;
}

QVariant PluginItem::data(int column) const
{
    if (column == 4) {
        if (m_parentItem == nullptr) {
            return QVariant(QObject::tr("Count"));
        } else {
            return childCount();
        }
    }

    return m_itemData.value(column);
}

PluginItem* PluginItem::parentItem()
{
    return m_parentItem;
}

PluginItem *PluginItem::find(const QString &path)
{
    if (path == m_path) {
        return this;
    }

    QStringList dpath = path.split("::");
    QStringList lpath = m_path.split("::");

    PluginItem *result = nullptr;

    if (lpath.length() < dpath.length()) {
        PluginItem *child = nullptr;
        foreach (child, m_childItems) {
            result = child->find(path);

            if (result != nullptr) {
                return result;
            }
        }
    }

    return nullptr;
}

const QString &PluginItem::name() const
{
    return m_name;
}

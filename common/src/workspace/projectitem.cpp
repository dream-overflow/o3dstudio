/**
 * @brief Application project item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#include "o3d/studio/common/workspace/projectitem.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


ProjectItem::ProjectItem(const QString &name, const QList<QVariant> &data, ProjectItem *parentItem) :
    m_name(name),
    m_path(""),
    m_itemData(data),
    m_parentItem(parentItem)
{
    if (parentItem) {
        m_path = parentItem->m_path + "::" + name;
    } else {
        m_path = name;
    }
}

ProjectItem::~ProjectItem()
{
    qDeleteAll(m_childItems);
}

void ProjectItem::appendChild(ProjectItem *item)
{
    m_childItems.append(item);
}

ProjectItem *ProjectItem::child(int row)
{
    return m_childItems.value(row);
}

int ProjectItem::childCount() const
{
    return m_childItems.count();
}

int ProjectItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<ProjectItem*>(this));
    }

    return 0;
}

int ProjectItem::columnCount() const
{
    return m_itemData.count();
}

QVariant ProjectItem::data(int column) const
{
    return m_itemData.value(column);
}

ProjectItem* ProjectItem::parentItem()
{
    return m_parentItem;
}

ProjectItem *ProjectItem::find(const QString &path)
{
    if (path == m_path) {
        return this;
    }

    QStringList dpath = path.split("::");
    QStringList lpath = m_path.split("::");

    ProjectItem *result = nullptr;

    if (lpath.length() < dpath.length()) {
        ProjectItem *child = nullptr;
        foreach (child, m_childItems) {
            result = child->find(path);

            if (result != nullptr) {
                return result;
            }
        }
    }

    return nullptr;
}

const QString &ProjectItem::name() const
{
    return m_name;
}

/**
 * @brief Application project item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#include "o3d/studio/common/workspace/projectitem.h"
#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


ProjectItem::ProjectItem(const QUuid &uuid, const QString &name, const QIcon &icon, ProjectItem *parentItem) :
    m_path(""),
    m_name(name),
    m_uuid(uuid),
    m_icon(icon),
    m_parentItem(parentItem)
{
    if (parentItem) {
        m_path = parentItem->m_path + "::" + uuid.toString();
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
    return 1;
}

QVariant ProjectItem::data(int column) const
{
    if (column == 0) {
        return m_name;
    } else {
        return QVariant();
    }
}

QVariant ProjectItem::decoration(int column) const
{
    if (column == 0) {
        return m_icon;
    } else {
        return QVariant(QIcon::fromTheme("applications-system"));
    }
}

ProjectItem* ProjectItem::parentItem()
{
    return m_parentItem;
}

void ProjectItem::removeChild(int row)
{
    if (row < 0 || row > m_childItems.size()) {
        return;
    }

    m_childItems.removeAt(row);
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

ProjectItem *ProjectItem::find(const QUuid &uuid)
{
    if (m_uuid == uuid) {
        return this;
    }

    ProjectItem *result = nullptr;
    ProjectItem *child = nullptr;
    foreach (child, m_childItems) {
        result = child->find(uuid);

        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const QString &ProjectItem::name() const
{
    return m_name;
}

const QUuid &ProjectItem::uuid() const
{
    return m_uuid;
}

const Project *ProjectItem::project() const
{
    Workspace *workspace = Application::instance()->workspaceManager().current();
    return workspace->project(m_uuid);
}

Project *ProjectItem::project()
{
    Workspace *workspace = Application::instance()->workspaceManager().current();
    return workspace->project(m_uuid);
}

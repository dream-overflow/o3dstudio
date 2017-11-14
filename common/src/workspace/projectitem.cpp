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
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/asset.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


ProjectItem::ProjectItem(Entity *entity,
                         const LightRef &ref,
                         const QString &name,
                         const QIcon &icon,
                         ProjectItem *parentItem) :
    m_path(""),
    m_name(name),
    m_icon(icon),
    m_ref(ref),
    m_parentItem(parentItem),
    m_entity(entity)
{
    if (parentItem) {
        m_path = parentItem->m_path + "::" + ref.longId();
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

ProjectItem *ProjectItem::find(const LightRef &ref)
{
    if (m_ref == ref) {
        return this;
    }

    ProjectItem *result = nullptr;
    ProjectItem *child = nullptr;
    foreach (child, m_childItems) {
        result = child->find(ref);

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

const LightRef& ProjectItem::ref() const
{
    return m_ref;
}

const Entity *ProjectItem::entity() const
{
    return m_entity;
}

Entity *ProjectItem::entity()
{
    return m_entity;
}

bool ProjectItem::isProject() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::project().id()) {
        return true;
    } else {
        return false;
    }
}

bool ProjectItem::isHub() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::hub().id()) {
        return true;
    } else {
        return false;
    }
}

bool ProjectItem::isFragment() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::fragment().id()) {
        return true;
    } else {
        return false;
    }
}

bool ProjectItem::isAsset() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::asset().id()) {
        return true;
    } else {
        return false;
    }
}

const Project *ProjectItem::project() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::project().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_ref) == m_entity) {
            return static_cast<const Project*>(m_entity);
        }
    }

    return nullptr;
}

Project *ProjectItem::project()
{
    if (m_ref.isValid() && m_entity && m_entity->typeRef() == TypeRef::project()) {
        Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_ref) == m_entity) {
            return static_cast<Project*>(m_entity);
        }
    }

    return nullptr;
}

const Hub *ProjectItem::hub() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::hub().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<const Hub*>(m_entity);
        }
    }

    return nullptr;
}

Hub *ProjectItem::hub()
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::hub().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<Hub*>(m_entity);
        }
    }

    return nullptr;
}

const Fragment *ProjectItem::fragment() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::fragment().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<const Fragment*>(m_entity);
        }
    }

    return nullptr;
}

Fragment *ProjectItem::fragment()
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::fragment().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<Fragment*>(m_entity);
        }
    }

    return nullptr;
}

const Asset *ProjectItem::asset() const
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::asset().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<const Asset*>(m_entity);
        }
    }

    return nullptr;
}

Asset *ProjectItem::asset()
{
    if (m_entity && m_ref.isValid() && m_entity->typeRef().baseType() == TypeRef::asset().id()) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<Asset*>(m_entity);
        }
    }

    return nullptr;
}

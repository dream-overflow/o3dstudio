/**
 * @brief Application project item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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
//#include "o3d/studio/common/workspace/asset.h"

#include "o3d/studio/common/ui/uiutils.h"

#include <QtCore/qobjectdefs.h>

using namespace o3d::studio::common;


ProjectItem::ProjectItem(Entity *entity,
                         const LightRef &ref,
                         const String &name,
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
    for (ProjectItem *item : m_childItems) {
        deletePtr(item);
    }

    m_childItems.clear();
}

void ProjectItem::appendChild(ProjectItem *item)
{
    m_childItems.push_back(item);
}

void ProjectItem::insertChild(ProjectItem *item, o3d::Int32 row)
{
    if (row < 0 || row >= (Int32)m_childItems.size()) {
        m_childItems.push_back(item);
    } else {
        Int32 i = 0;
        auto it = m_childItems.begin();
        while (i < row) {
            ++i;
            ++it;

            if (it == m_childItems.end()) {
                break;
            }
        }

        m_childItems.emplace(it, item);
    }
}

ProjectItem *ProjectItem::child(Int32 row)
{
    Int32 i = 0;
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

o3d::Int32 ProjectItem::childCount() const
{
    return (Int32)m_childItems.size();
}

o3d::Int32 ProjectItem::row() const
{
    if (m_parentItem) {
        Int32 n = 0;
        for (const ProjectItem* item : m_parentItem->m_childItems) {
            if (item == this) {
                return n;
            }
            ++n;
        }
    }

    return 0;
}

o3d::Int32 ProjectItem::columnCount() const
{
    return 1;
}

QVariant ProjectItem::data(Int32 column) const
{
    if (column == 0) {
        return toQString(m_name);
    } else {
        return QVariant();
    }
}

QVariant ProjectItem::decoration(Int32 column) const
{
    if (column == 0) {
        return m_icon;
    } else {
        return QVariant(QIcon::fromTheme("applications-system"));
    }
}

o3d::Bool ProjectItem::setData(o3d::Int32 column, const QVariant &value)
{
    if (column == 0) {
        m_name = fromQString(value.toString());
        return True;
    }

    return False;
}

ProjectItem* ProjectItem::parentItem()
{
    return m_parentItem;
}

void ProjectItem::removeChild(Int32 row)
{
    if (row < 0 || row > (Int32)m_childItems.size()) {
        return;
    }

    Int32 i = 0;
    auto it = m_childItems.begin();
    while (i < row) {
        ++it;
        ++i;

        if (it == m_childItems.end()) {
            return;
        }
    }

    m_childItems.erase(it);
}

ProjectItem *ProjectItem::find(const String &path)
{
    if (path == m_path) {
        return this;
    }

    QStringList dpath = toQString(path).split("::");
    QStringList lpath = toQString(m_path).split("::");

    ProjectItem *result = nullptr;

    if (lpath.length() < dpath.length()) {
        for (ProjectItem *child : m_childItems) {
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
    for (ProjectItem *child : m_childItems) {
        result = child->find(ref);

        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

const o3d::String &ProjectItem::name() const
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

void ProjectItem::updatePresentation()
{
    if (m_entity && m_ref.isValid()) {
        m_name = m_entity->name();
    }
}

o3d::Bool ProjectItem::isProject() const
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::project())) {
        return True;
    } else {
        return False;
    }
}

o3d::Bool ProjectItem::isHub() const
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::hub())) {
        return True;
    } else {
        return False;
    }
}

o3d::Bool ProjectItem::isFragment() const
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::fragment())) {
        return True;
    } else {
        return False;
    }
}

//o3d::Bool ProjectItem::isAsset() const
//{
//    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::asset())) {
//        return True;
//    } else {
//        return False;
//    }
//}

const Project *ProjectItem::project() const
{
    if (m_entity && m_ref.isValid()) {
        if (m_ref.baseTypeOf(TypeRef::project())) {
            // entity is a project
            const Workspace *workspace = Application::instance()->workspaces().current();
            if (workspace->project(m_ref) == m_entity) {
                return static_cast<const Project*>(m_entity);
            }
        } else {
            // get the project of the entity
            const Workspace *workspace = Application::instance()->workspaces().current();
            const Project *project = workspace->project(m_ref);

            if (m_entity->project() == project) {
                return project;
            }
        }
    }

    return nullptr;
}

Project *ProjectItem::project()
{
    if (m_entity && m_ref.isValid()) {
        if (m_ref.baseTypeOf(TypeRef::project())) {
            // entity is a project
            Workspace *workspace = Application::instance()->workspaces().current();
            if (workspace->project(m_ref) == m_entity) {
                return static_cast<Project*>(m_entity);
            }
        } else {
            // get the project of the entity
            Workspace *workspace = Application::instance()->workspaces().current();
            Project *project = workspace->project(m_ref);

            if (m_entity->project() == project) {
                return project;
            }
        }
    }

    return nullptr;
}

const Hub *ProjectItem::hub() const
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::hub())) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<const Hub*>(m_entity);
        }
    }

    return nullptr;
}

Hub *ProjectItem::hub()
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::hub())) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<Hub*>(m_entity);
        }
    }

    return nullptr;
}

const Fragment *ProjectItem::fragment() const
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::fragment())) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<const Fragment*>(m_entity);
        }
    }

    return nullptr;
}

Fragment *ProjectItem::fragment()
{
    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::fragment())) {
        const Workspace *workspace = Application::instance()->workspaces().current();
        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
                return static_cast<Fragment*>(m_entity);
        }
    }

    return nullptr;
}

void ProjectItem::setSelected(o3d::Bool selected)
{
    if (selected) {
        m_icon = common::UiUtils::tintIcon(m_icon, -1, nullptr, common::UiUtils::TINT_HIGHLIGHT);
    } else {
        m_icon = common::UiUtils::tintIcon(m_icon, -1, nullptr, common::UiUtils::TINT_NORMAL);
    }
}

//const Asset *ProjectItem::asset() const
//{
//    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::asset())) {
//        const Workspace *workspace = Application::instance()->workspaces().current();
//        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
//                return static_cast<const Asset*>(m_entity);
//        }
//    }

//    return nullptr;
//}

//Asset *ProjectItem::asset()
//{
//    if (m_entity && m_ref.isValid() && m_ref.baseTypeOf(TypeRef::asset())) {
//        const Workspace *workspace = Application::instance()->workspaces().current();
//        if (workspace->project(m_entity->project()->ref().light()) == m_entity->project()) {
//                return static_cast<Asset*>(m_entity);
//        }
//    }

//    return nullptr;
//}

/**
 * @brief Application project model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/projectitem.h"

#include "o3d/studio/common/workspace/project.h"

using namespace o3d::studio::common;

ProjectModel::ProjectModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new ProjectItem(LightRef(), "o3s::workspace", QIcon());
}

ProjectModel::~ProjectModel()
{
    if (m_rootItem) {
        delete m_rootItem;
    }
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    ProjectItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());
    }

    ProjectItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex ProjectModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    ProjectItem *childItem = static_cast<ProjectItem*>(child.internalPointer());
    ProjectItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    ProjectItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<ProjectItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<ProjectItem*>(parent.internalPointer())->columnCount();
    } else {
        return m_rootItem->columnCount();
    }
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    ProjectItem *item = static_cast<ProjectItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    } else if (role == Qt::DecorationRole) {
        return item->decoration(index.column());
    }
/*
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
    } else {*/
        return QVariant();
    //}
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)

    // no header
    return QVariant();
}

const ProjectItem *ProjectModel::find(const LightRef &ref) const
{
    return m_rootItem->find(ref);
}

ProjectItem *ProjectModel::find(const LightRef &ref)
{
    return m_rootItem->find(ref);
}

ProjectItem *ProjectModel::addProject(Project *project)
{
    if (!project) {
        return nullptr;
    }

    int n = m_rootItem->childCount();

    beginInsertRows(QModelIndex(), n, n);

    ProjectItem *item = new ProjectItem(project->ref().light(), project->name(), QIcon::fromTheme("document-open"), m_rootItem);
    m_rootItem->appendChild(item);

    endInsertRows();

    return item;
}

void ProjectModel::removeProject(Project *project)
{
    if (!project) {
        return;
    }

    // find the project
    ProjectItem *projectItem = m_rootItem->find(project->ref().light());
    if (!projectItem) {
        return;
    }

    Project *lproject = m_rootItem->project();
    if (project != lproject) {
        return;
    }

    int n = projectItem->row();

    beginRemoveRows(QModelIndex(), n, n);
    m_rootItem->removeChild(n);

    delete projectItem;

    endRemoveRows();
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
/*
ProjectItem* createItem(const QString &section, const QString &name, const QString &label, ProjectItem *itemParent)
{
    QList<QVariant> itemData;
    itemData << label << QString() << QString();

    ProjectItem *item = new ProjectItem(section, name, itemData, itemParent);
    return item;
}*/
/*
void ProjectModel::setupModelData(const QList<PluginSection *> &data, ProjectItem *parent)
{
    PluginItem *rootItem = m_rootItem;

    PluginSection *section = nullptr;
    PluginItem *itemParent = nullptr;

    foreach (section, data) {
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
*/

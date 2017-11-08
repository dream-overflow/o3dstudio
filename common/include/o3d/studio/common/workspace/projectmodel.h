/**
 * @brief Application project model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTMODEL_H
#define _O3DS_COMMON_PROJECTMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

#include "projectitem.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
// class Hub;

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit ProjectModel(QObject *parent = nullptr);
    virtual ~ProjectModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    // virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    // virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const ProjectItem *find(int type, const QUuid &uuid) const;
    ProjectItem *find(int type, const QUuid &uuid);

    ProjectItem *addProject(Project *project);
    void removeProject(Project *project);

    //ProjectItem *addHub(Hub *hub);

private:

    ProjectItem *m_rootItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTMODEL_H

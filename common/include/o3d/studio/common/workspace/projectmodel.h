/**
 * @brief Application project model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTMODEL_H
#define _O3DS_COMMON_PROJECTMODEL_H

#include <QtCore/QAbstractItemModel>

#include <list>

#include "projectitem.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
class Hub;
class Fragment;
//class Asset;

class O3S_API ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit ProjectModel(QObject *parent = nullptr);
    virtual ~ProjectModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;

    QModelIndex modelIndex(ProjectItem *item) const;

    // virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual Bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const ProjectItem *find(const LightRef &ref) const;
    ProjectItem *find(const LightRef &ref);

    ProjectItem *addProject(Project *project);
    void removeProject(const common::LightRef &ref);

    ProjectItem *addHub(common::Hub *hub);
    void removeHub(const common::LightRef &ref);

    ProjectItem *addFragment(common::Fragment *fragment);
    void removeFragment(const common::LightRef &ref);

//    ProjectItem *addAsset(common::Asset *asset);
//    void removeAsset(const common::LightRef &ref);

    void updatePresentation(const LightRef &ref);
    void updatePresentation(QModelIndex itemIndex);

private:

    ProjectItem *m_rootItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTMODEL_H

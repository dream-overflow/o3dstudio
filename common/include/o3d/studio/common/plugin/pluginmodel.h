/**
 * @brief Application plugin model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-28
 * @details
 */

#ifndef _O3DS_COMMON_PLUGINMODEL_H
#define _O3DS_COMMON_PLUGINMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

#include "pluginitem.h"

namespace o3d {
namespace studio {
namespace common {

class PluginSection;

class O3S_API PluginModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit PluginModel(const QList<PluginSection*> &data, QObject *parent = nullptr);
    virtual ~PluginModel();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    // virtual QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    // virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:

    void setupModelData(const QList<PluginSection*> &data, PluginItem *parent);

    PluginItem *m_rootItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PLUGINMODEL_H

/**
 * @brief Application property model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYMODEL_H
#define _O3DS_COMMON_PROPERTYMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

#include "propertyitem.h"

namespace o3d {
namespace studio {
namespace common {

class PropertySection;

class O3S_API PropertyModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit PropertyModel(const QList<PropertySection*> &data, QObject *parent = nullptr);
    virtual ~PropertyModel();

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

    void setupModelData(const QList<PropertySection*> &data, PropertyItem *parent);

    PropertyItem *m_rootItem;
    QList<PropertySection*> m_sections;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYMODEL_H

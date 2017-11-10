/**
 * @brief Application property item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYITEM_H
#define _O3DS_COMMON_PROPERTYITEM_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class PropertySection;

class O3S_API PropertyItem
{
public:

    PropertyItem(const QString section, const QList<QVariant> &data, PropertySection *ps = nullptr, PropertyItem *parentItem = nullptr);
    virtual ~PropertyItem();

    void appendChild(PropertyItem *child);

    PropertyItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PropertyItem *parentItem();

    PropertyItem *find(const QString &path);

    PropertySection *section();
    const PropertySection *section() const;

    const QString& path() const;

private:

    QString m_path;

    // QDataWidgetMapper
    QList<PropertyItem*> m_childItems;
    QList<QVariant> m_itemData;
    PropertyItem *m_parentItem;

    PropertySection *m_section;   //! Related property widget to be setup into the property frame
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYITEM_H

/**
 * @brief Application property item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYITEM_H
#define _O3DS_COMMON_PROPERTYITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QtWidgets>

#include <list>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class PropertySection;

class O3S_API PropertyItem
{
public:

    PropertyItem(const String section, const String &label, PropertySection *ps = nullptr, PropertyItem *parentItem = nullptr);
    virtual ~PropertyItem();

    void appendChild(PropertyItem *child);

    PropertyItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PropertyItem *parentItem();

    PropertyItem *find(const String &path);

    PropertySection *section();
    const PropertySection *section() const;

    const String& path() const;

private:

    String m_path;

    // QDataWidgetMapper
    std::list<PropertyItem*> m_childItems;
    String m_label;
    PropertyItem *m_parentItem;

    PropertySection *m_section;   //! Related property widget to be setup into the property frame
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYITEM_H

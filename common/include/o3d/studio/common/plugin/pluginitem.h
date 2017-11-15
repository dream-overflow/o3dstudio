/**
 * @brief Application plugin item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-28
 * @details
 */

#ifndef _O3DS_COMMON_PLUGINITEM_H
#define _O3DS_COMMON_PLUGINITEM_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class PluginSection;

class O3S_API PluginItem
{
public:

    PluginItem(const QString &section, const QString &name, const QList<QVariant> &data, PluginItem *parentItem = nullptr);
    virtual ~PluginItem();

    void appendChild(PluginItem *child);

    PluginItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    PluginItem *parentItem();

    PluginItem *find(const QString &path);

    const QString& name() const;

private:

    QString m_path;
    QString m_name;

    // QDataWidgetMapper
    QList<PluginItem*> m_childItems;
    QList<QVariant> m_itemData;
    PluginItem *m_parentItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PLUGINITEM_H

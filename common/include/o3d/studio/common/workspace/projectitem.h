/**
 * @brief Application project item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTITEM_H
#define _O3DS_COMMON_PROJECTITEM_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

namespace o3d {
namespace studio {
namespace common {

class ProjectItem
{
public:

    ProjectItem(const QString &name, const QList<QVariant> &data, ProjectItem *parentItem = nullptr);
    virtual ~ProjectItem();

    void appendChild(ProjectItem *child);

    ProjectItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ProjectItem *parentItem();

    ProjectItem *find(const QString &path);

    const QString& name() const;

private:

    QString m_path;
    QString m_name;

    // QDataWidgetMapper
    QList<ProjectItem*> m_childItems;
    QList<QVariant> m_itemData;
    ProjectItem *m_parentItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTITEM_H

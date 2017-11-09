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

#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
// class Hub;

class ProjectItem
{
public:

    ProjectItem(const LightRef &ref, const QString &name, const QIcon &icon, ProjectItem *parentItem = nullptr);
    virtual ~ProjectItem();

    void appendChild(ProjectItem *child);

    ProjectItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    QVariant decoration(int column) const;
    int row() const;
    ProjectItem *parentItem();

    void removeChild(int row);

    ProjectItem *find(const QString &path);
    ProjectItem *find(const LightRef &ref);

    const QString& name() const;
    const LightRef& ref() const;

    const Project* project() const;
    Project* project();

//    const Hub* hub() const;
//    Hub* hub();

private:

    QString m_path;
    QString m_name;
    LightRef m_ref;
    QIcon m_icon;

    // QDataWidgetMapper
    QList<ProjectItem*> m_childItems;
    ProjectItem *m_parentItem;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTITEM_H

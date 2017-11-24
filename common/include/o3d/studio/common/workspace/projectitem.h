/**
 * @brief Application project item for model
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTITEM_H
#define _O3DS_COMMON_PROJECTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QtWidgets>

#include <list>

#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;
class Project;
class Hub;
class Fragment;
class Asset;

class O3S_API ProjectItem
{
public:

    ProjectItem(Entity *entity,
                const LightRef &ref,
                const String &name,
                const QIcon &icon,
                ProjectItem *parentItem = nullptr);

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

    ProjectItem *find(const String &path);
    ProjectItem *find(const LightRef &ref);

    const String& name() const;
    const LightRef& ref() const;

    const Entity* entity() const;
    Entity* entity();

    Bool isProject() const;
    Bool isHub() const;
    Bool isFragment() const;
    Bool isAsset() const;

    TypeRef typeRef() const;

    const Project* project() const;
    Project* project();

    const Hub* hub() const;
    Hub* hub();

    const Fragment* fragment() const;
    Fragment* fragment();

    const Asset* asset() const;
    Asset* asset();

private:

    String m_path;
    String m_name;
    QIcon m_icon;

    LightRef m_ref;

    // QDataWidgetMapper
    std::list<ProjectItem*> m_childItems;
    ProjectItem *m_parentItem;

    Entity *m_entity;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTITEM_H

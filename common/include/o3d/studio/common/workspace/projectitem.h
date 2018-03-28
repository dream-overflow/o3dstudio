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
//class Asset;

class O3S_API ProjectItem
{
public:

    ProjectItem(Entity *entity,
                const LightRef &ref,
                const String &name,
                const QIcon &icon,
                ProjectItem *parentItem = nullptr);

    virtual ~ProjectItem();

    void appendChild(ProjectItem *item);
    void insertChild(ProjectItem *item, Int32 row);

    ProjectItem *child(Int32 row);
    Int32 childCount() const;
    Int32 columnCount() const;
    QVariant data(Int32 column) const;
    QVariant decoration(Int32 column) const;
    Bool setData(Int32 column, const QVariant &value);
    Int32 row() const;
    ProjectItem *parentItem();

    void removeChild(Int32 row);

    ProjectItem *find(const String &path);
    ProjectItem *find(const LightRef &ref);

    const String& name() const;
    const LightRef& ref() const;

    const Entity* entity() const;
    Entity* entity();

    void updatePresentation();

    //
    // Entity
    //

    Bool isProject() const;
    Bool isHub() const;
    Bool isFragment() const;
//    Bool isAsset() const;

    TypeRef typeRef() const;

    const Project* project() const;
    Project* project();

    const Hub* hub() const;
    Hub* hub();

    const Fragment* fragment() const;
    Fragment* fragment();

//    const Asset* asset() const;
//    Asset* asset();

    //
    // Properties
    //

    /**
     * @brief Set selected state and look'n'fell for the item.
     */
    void setSelected(Bool selected);

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

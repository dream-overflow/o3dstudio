/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-0
 * @details
 */

#ifndef _O3DS_COMMON_SELECTIONITEM_H
#define _O3DS_COMMON_SELECTIONITEM_H

#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;

/**
 * @brief The SelectionItem used by the selection manager.
 * The state selected of the entity is not managed by selection item but by the selection manager.
 */
class O3S_API SelectionItem
{
public:

    SelectionItem(Entity *entity);
    SelectionItem(const LightRef &ref);
    SelectionItem(const LightRef &ref, const LightRef &parentRef);

    ~SelectionItem();

    inline const LightRef& ref() const { return m_ref; }
    inline const LightRef& parentRef() const { return m_parentRef; }

    const Entity *entity() const;
    Entity *entity();

    void select();
    void unselect();

private:

    LightRef m_parentRef;
    LightRef m_ref;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTIONITEM_H

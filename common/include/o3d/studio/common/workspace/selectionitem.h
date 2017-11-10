/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-0
 * @details
 */

#ifndef _O3DS_COMMON_SELECTIONITEM_H
#define _O3DS_COMMON_SELECTIONITEM_H

#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API SelectionItem
{
public:

    enum SelectionType
    {
        SELECTION_PROJECT = 0,
        SELECTION_FRAGMENT = 1,
        SELECTION_HUB = 2
    };

    SelectionItem(SelectionType type, const LightRef &ref);
    ~SelectionItem();

    const LightRef& ref() const;
    SelectionType selectionType() const;

private:

    SelectionType m_type;
    LightRef m_ref;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTIONITEM_H

/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-0
 * @details
 */

#ifndef _O3DS_COMMON_SELECTIONITEM_H
#define _O3DS_COMMON_SELECTIONITEM_H

#include <QtCore/QUuid>

namespace o3d {
namespace studio {
namespace common {

class SelectionItem
{
public:

    enum SelectionType
    {
        SELECTION_PROJECT = 0,
        SELECTION_HUB = 1
    };

    SelectionItem(SelectionType type, const QUuid uuid);
    ~SelectionItem();

    const QUuid uuid() const;
    SelectionType selectionType() const;

private:

    SelectionType m_type;
    QUuid m_uuid;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTIONITEM_H

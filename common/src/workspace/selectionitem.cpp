/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-0
 * @details
 */

#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;

SelectionItem::SelectionItem(SelectionItem::SelectionType type, const QUuid uuid) :
    m_type(type),
    m_uuid(uuid)
{

}

SelectionItem::~SelectionItem()
{

}

const QUuid SelectionItem::uuid() const
{
    return m_uuid;
}

SelectionItem::SelectionType SelectionItem::selectionType() const
{
    return m_type;
}

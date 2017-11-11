/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-0
 * @details
 */

#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;

SelectionItem::SelectionItem(const LightRef &ref) :
    m_ref(ref)
{

}

SelectionItem::~SelectionItem()
{

}

const LightRef& SelectionItem::ref() const
{
    return m_ref;
}

qint64 SelectionItem::selectionType() const
{
    return m_ref.type();
}

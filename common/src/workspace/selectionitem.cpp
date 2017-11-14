/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-0
 * @details
 */

#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::common;

SelectionItem::SelectionItem(const LightRef &ref, const TypeRef &typeRef) :
    m_parentTypeRef(),
    m_parentRef(),
    m_ref(ref),
    m_typeRef(typeRef)
{

}

SelectionItem::SelectionItem(const LightRef &ref,
                             const TypeRef &typeRef,
                             const LightRef &parentRef,
                             const TypeRef &parentTypeRef) :
    m_parentTypeRef(parentTypeRef),
    m_parentRef(parentRef),
    m_typeRef(typeRef),
    m_ref(ref)
{

}

SelectionItem::~SelectionItem()
{

}

qint64 SelectionItem::selectionType() const
{
    return m_ref.type();
}

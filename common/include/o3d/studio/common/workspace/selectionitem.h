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

    SelectionItem(const LightRef &ref, const TypeRef &typeRef);
    SelectionItem(const LightRef &ref, const TypeRef &typeRef, const LightRef &parentRef, const TypeRef &parentTypeRef);

    ~SelectionItem();

    inline const LightRef& ref() const { return m_ref; }
    inline const TypeRef& typeRef() const { return m_typeRef; }
    inline const LightRef& parentRef() const { return m_parentRef; }
    inline const TypeRef& parentTypeRef() const { return m_parentTypeRef; }

    qint64 selectionType() const;

private:

    TypeRef m_parentTypeRef;
    LightRef m_parentRef;

    TypeRef m_typeRef;
    LightRef m_ref;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTIONITEM_H

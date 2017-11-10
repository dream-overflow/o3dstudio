/**
 * @brief Common component base abstract class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENT_H
#define _O3DS_COMMON_COMPONENT_H

#include <QtWidgets/QWidget>
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API Component
{
public:

    Component();
    virtual ~Component();

    void setTypeRef(TypeRef& typeRef);
    const TypeRef& typeRef() const;

    const QString& name() const;

    // @todo soit ComponentUi object or directly label, icon.. action ?
    // const QString& label() const;

protected:

    TypeRef m_typeRef;
    QString m_name;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENT_H

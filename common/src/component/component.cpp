/**
 * @brief Common component base abstract class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#include "o3d/studio/common/component/component.h"

using namespace o3d::studio::common;


Component::Component()
{

}

Component::~Component()
{

}

void Component::setTypeRef(TypeRef &typeRef)
{
    m_typeRef = typeRef;
}

const TypeRef &Component::typeRef() const
{
    return m_typeRef;
}

const QString &Component::name() const
{
    return m_name;
}

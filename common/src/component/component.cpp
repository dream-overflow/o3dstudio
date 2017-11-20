/**
 * @brief Common component base abstract class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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

void Component::setTypeRef(const TypeRef &typeRef)
{
    m_typeRef = typeRef;
}

const TypeRef &Component::typeRef() const
{
    return m_typeRef;
}

const o3d::String &Component::name() const
{
    return m_name;
}

const o3d::String &Component::targetName() const
{
    return m_targetName;
}

void Component::setTargetTypeRef(const  TypeRef &typeRef)
{
    m_targetTypeRef = typeRef;
}

const TypeRef &Component::targetTypeRef() const
{
    return m_targetTypeRef;
}

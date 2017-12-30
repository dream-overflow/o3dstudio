/**
 * @brief Common builder base class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#include "o3d/studio/common/builder/builder.h"

using namespace o3d::studio::common;

BuilderOption::BuilderOption()
{

}

BuilderOption::~BuilderOption()
{

}

o3d::Bool BuilderOption::readFromFile(o3d::InStream &istream)
{
    return True;
}

o3d::Bool BuilderOption::writeToFile(o3d::OutStream &ostream)
{
    return True;
}

Builder::Builder()
{

}

Builder::~Builder()
{

}

void Builder::setTypeRef(const TypeRef &typeRef)
{
    m_typeRef = typeRef;
}

const TypeRef &Builder::typeRef() const
{
    return m_typeRef;
}

const o3d::String &Builder::name() const
{
    return m_name;
}

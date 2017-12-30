/**
 * @brief Common importer base class.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#include "o3d/studio/common/importer/importer.h"

using namespace o3d::studio::common;

ImporterOption::ImporterOption()
{

}

ImporterOption::~ImporterOption()
{

}

o3d::Bool ImporterOption::readFromFile(o3d::InStream &istream)
{
    return True;
}

o3d::Bool ImporterOption::writeToFile(o3d::OutStream &ostream)
{
    return True;
}

Importer::Importer()
{

}

Importer::~Importer()
{

}

void Importer::setTypeRef(const TypeRef &typeRef)
{
    m_typeRef = typeRef;
}

const TypeRef &Importer::typeRef() const
{
    return m_typeRef;
}

const o3d::String &Importer::name() const
{
    return m_name;
}

const o3d::BitSet32 &Importer::capacities() const
{
    return m_capacities;
}

o3d::Bool Importer::getCapacity(Importer::ImporterFeature feature) const
{
    return m_capacities.getBit(feature);
}

/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#include <QtWidgets/QWidget>

#include "o3d/studio/common/property/propertysectionnode.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

using namespace o3d::studio::common;

PropertySection::~PropertySection()
{
}

PropertySectionNode::PropertySectionNode(const String &section, const String &label) :
    m_name(section),
    m_label(label)
{
}

PropertySectionNode::~PropertySectionNode()
{
}

o3d::String PropertySectionNode::name()
{
    return m_name;
}

o3d::String PropertySectionNode::label()
{
    return m_label;
}

o3d::Bool PropertySectionNode::setupSection(QWidget */*parent*/)
{
    return true;
}

o3d::Bool PropertySectionNode::cleanupSection(QWidget */*parent*/)
{
    return true;
}

void PropertySectionNode::commit()
{

}

o3d::Bool PropertySectionNode::save()
{
    return true;
}

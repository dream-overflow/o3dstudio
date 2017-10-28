/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#include <QtWidgets/QWidget>

#include "common/property/propertysectionnode.h"

#include "common/application.h"
#include "common/settings.h"

using namespace o3d::studio::common;

PropertySection::~PropertySection()
{
}

PropertySectionNode::PropertySectionNode(const QString &section, const QString &label) :
    m_name(section),
    m_label(label)
{
}

PropertySectionNode::~PropertySectionNode()
{
}

QString PropertySectionNode::name()
{
    return m_name;
}

QString PropertySectionNode::label()
{
    return m_label;
}

bool PropertySectionNode::setupSection(QWidget *parent)
{
    return true;
}

bool PropertySectionNode::cleanupSection(QWidget *parent)
{
    return true;
}

void PropertySectionNode::commit()
{

}

bool PropertySectionNode::save()
{
    return true;
}

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
    Q_UNUSED(parent)
    return true;
}

bool PropertySectionNode::cleanupSection(QWidget *parent)
{
    Q_UNUSED(parent)
    return true;
}

void PropertySectionNode::commit()
{

}

bool PropertySectionNode::save()
{
    return true;
}

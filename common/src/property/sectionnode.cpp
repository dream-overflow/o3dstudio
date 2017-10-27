/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#include <QtWidgets/QWidget>

#include "common/property/sectionnode.h"

#include "common/application.h"
#include "common/settings.h"

using namespace o3d::studio::common;


SectionNode::SectionNode(const QString &section, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_name(section),
    m_label(label)
{
}

SectionNode::~SectionNode()
{
}

QString SectionNode::name()
{
    return m_name;
}

QString SectionNode::label()
{
    return m_label;
}

bool SectionNode::setupSection(QWidget *parent)
{
    return true;
}

void SectionNode::commit()
{

}

bool SectionNode::save()
{
    return true;
}

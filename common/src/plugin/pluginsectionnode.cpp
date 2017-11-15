/**
 * @brief Application display plugin section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-28
 * @details
 */

#include <QtWidgets/QWidget>

#include "o3d/studio/common/plugin/pluginsectionnode.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

using namespace o3d::studio::common;


PluginSectionNode::PluginSectionNode(const QString &section, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_name(section),
    m_label(label)
{
}

PluginSectionNode::~PluginSectionNode()
{
}

QString PluginSectionNode::name() const
{
    return m_name;
}

QString PluginSectionNode::label() const
{
    return m_label;
}

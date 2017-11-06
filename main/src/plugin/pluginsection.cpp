/**
 * @brief Application display plugin section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-28
 * @details
 */

#include <QtWidgets/QWidget>

#include "pluginsection.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

using namespace o3d::studio::main;


PluginSection::PluginSection(const QString &name, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_name(name),
    m_label(label)
{
}

PluginSection::~PluginSection()
{
}

QString PluginSection::name() const
{
    return "o3s::plugin::" + m_name;
}

QString PluginSection::label() const
{
    return m_label;
}

/**
 * @brief Panel property abstract model.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-21
 * @details
 */

#include "o3d/studio/common/ui/property/panelproperty.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

using namespace o3d::studio::common;


PanelProperty::PanelProperty(Panel *panel, const String &name, const o3d::String &label) :
    EvtHandler(),
    m_name(name),
    m_label(label),
    m_panel(panel)
{
    O3D_ASSERT(m_panel != nullptr);
}

PanelProperty::~PanelProperty()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *PanelProperty::ui()
{
    return m_widget;
}

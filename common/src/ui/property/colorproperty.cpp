/**
 * @brief Panel property with 4 components vector.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <QDoubleSpinBox>

#include "o3d/studio/common/ui/property/colorproperty.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


ColorProperty::ColorProperty(
        Panel *panel,
        const String &name,
        const o3d::String &label,
        ColorFormat format) :
    PanelProperty(panel, name, label),
    m_format(format)
{
    QVBoxLayout *l = new QVBoxLayout();
    l->setContentsMargins(0, 0, 0, 1);
    l->setSpacing(0);

    QWidget *valueGroup = new QWidget();
    valueGroup->setLayout(l);

    l->addWidget(new QLabel(toQString(label)));

    QVBoxLayout *pl = new QVBoxLayout();
    pl->setContentsMargins(0, 0, 0, 1);
    pl->setSpacing(0);

    QWidget *pw = new QWidget();
    pw->setLayout(pl);

    // r
    m_v[0] = new QDoubleSpinBox();
    m_v[0]->setRange(0, 1);
    m_v[0]->setDecimals(6);
    m_v[0]->setSingleStep(0.1);
    m_v[0]->setPrefix("r:");
    m_v[0]->setValue(0);
    m_v[0]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[0]->setMinimumWidth(75);
    pl->addWidget(m_v[0]);

    // g
    m_v[1] = new QDoubleSpinBox();
    m_v[1]->setRange(0, 1);
    m_v[1]->setDecimals(6);
    m_v[1]->setSingleStep(0.1);
    m_v[1]->setPrefix("g:");
    m_v[1]->setValue(0);
    m_v[1]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[1]->setMinimumWidth(75);
    pl->addWidget(m_v[1]);

    // b
    m_v[2] = new QDoubleSpinBox();
    m_v[2]->setRange(0, 1);
    m_v[2]->setDecimals(6);
    m_v[2]->setSingleStep(0.1);
    m_v[2]->setPrefix("b:");
    m_v[2]->setValue(0);
    m_v[2]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[2]->setMinimumWidth(75);
    pl->addWidget(m_v[2]);

    // a
    m_v[3] = new QDoubleSpinBox();
    m_v[3]->setRange(0, 1);
    m_v[3]->setDecimals(6);
    m_v[3]->setSingleStep(0.1);
    m_v[3]->setPrefix("a:");
    m_v[3]->setValue(0);
    m_v[3]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[3]->setMinimumWidth(75);
    pl->addWidget(m_v[3]);

    l->addWidget(pw);
    m_widget = valueGroup;
}

ColorProperty::ColorFormat ColorProperty::colorFormat() const
{
    return m_format;
}

ColorProperty::~ColorProperty()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *ColorProperty::ui()
{
    return m_widget;
}

o3d::Color ColorProperty::value() const
{
    if (m_v[0]) {
        return Color(m_v[0]->value(), m_v[1]->value(), m_v[2]->value(), m_v[3]->value());
    } else {
        return o3d::Color();
    }
}

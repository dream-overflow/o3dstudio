/**
 * @brief Panel property with 4 components vector.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <QDoubleSpinBox>

#include "o3d/studio/common/ui/property/vector4property.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


Vector4Property::Vector4Property(Panel *panel, const String &name, const o3d::String &label) :
    PanelProperty(panel, name, label)
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

    // x
    m_v[0] = new QDoubleSpinBox();
    m_v[0]->setRange(-1000000000, 1000000000);
    m_v[0]->setDecimals(6);
    m_v[0]->setSingleStep(0.1);
    m_v[0]->setPrefix("x:");
    m_v[0]->setValue(0);
    m_v[0]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[0]->setMinimumWidth(75);
    pl->addWidget(m_v[0]);

    // y
    m_v[1] = new QDoubleSpinBox();
    m_v[1]->setRange(-1000000000, 1000000000);
    m_v[1]->setDecimals(6);
    m_v[1]->setSingleStep(0.1);
    m_v[1]->setPrefix("y:");
    m_v[1]->setValue(0);
    m_v[1]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[1]->setMinimumWidth(75);
    pl->addWidget(m_v[1]);

    // z
    m_v[2] = new QDoubleSpinBox();
    m_v[2]->setRange(-1000000000, 1000000000);
    m_v[2]->setDecimals(6);
    m_v[2]->setSingleStep(0.1);
    m_v[2]->setPrefix("z:");
    m_v[2]->setValue(0);
    m_v[2]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[2]->setMinimumWidth(75);
    pl->addWidget(m_v[2]);

    // w
    m_v[3] = new QDoubleSpinBox();
    m_v[3]->setRange(-1000000000, 1000000000);
    m_v[3]->setDecimals(6);
    m_v[3]->setSingleStep(0.1);
    m_v[3]->setPrefix("w:");
    m_v[3]->setValue(0);
    m_v[3]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[3]->setMinimumWidth(75);
    pl->addWidget(m_v[3]);

    l->addWidget(pw);
    m_widget = valueGroup;
}

Vector4Property::~Vector4Property()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *Vector4Property::ui()
{
    return m_widget;
}

void Vector4Property::setMinMax(o3d::Float min, o3d::Float max)
{
    if (m_v[0]) {
        m_v[0]->setMinimum(min);
        m_v[0]->setMaximum(max);

        m_v[1]->setMinimum(min);
        m_v[1]->setMaximum(max);

        m_v[2]->setMinimum(min);
        m_v[2]->setMaximum(max);

        m_v[3]->setMinimum(min);
        m_v[3]->setMaximum(max);
    }
}

o3d::Vector4f Vector4Property::value() const
{
    if (m_v[0]) {
        return Vector4f(m_v[0]->value(), m_v[1]->value(), m_v[2]->value(), m_v[3]->value());
    } else {
        return o3d::Vector4f(0, 0, 0, 0);
    }
}

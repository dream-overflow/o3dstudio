/**
 * @brief Vector3 panel property abstract model.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-04
 * @details
 */

#include <QDoubleSpinBox>

#include "o3d/studio/common/ui/property/vector3property.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


Vector3Property::Vector3Property(Panel *panel, const String &name, const o3d::String &label) :
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
    m_v[0]->blockSignals(true);
    m_v[0]->setRange(-1000000000, 1000000000);
    m_v[0]->setDecimals(6);
    m_v[0]->setSingleStep(0.1);
    m_v[0]->setPrefix("x:");
    m_v[0]->setValue(0);
    m_v[0]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[0]->setMinimumWidth(75);
    m_v[0]->blockSignals(false);
    pl->addWidget(m_v[0]);

    // y
    m_v[1] = new QDoubleSpinBox();
    m_v[1]->blockSignals(true);
    m_v[1]->setRange(-1000000000, 1000000000);
    m_v[1]->setDecimals(6);
    m_v[1]->setSingleStep(0.1);
    m_v[1]->setPrefix("y:");
    m_v[1]->setValue(0);
    m_v[1]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[1]->setMinimumWidth(75);
    m_v[1]->blockSignals(false);
    pl->addWidget(m_v[1]);

    // z
    m_v[2] = new QDoubleSpinBox();
    m_v[2]->blockSignals(true);
    m_v[2]->setRange(-1000000000, 1000000000);
    m_v[2]->setDecimals(6);
    m_v[2]->setSingleStep(0.1);
    m_v[2]->setPrefix("z:");
    m_v[2]->setValue(0);
    m_v[2]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_v[2]->setMinimumWidth(75);
    m_v[2]->blockSignals(false);
    pl->addWidget(m_v[2]);

    l->addWidget(pw);
    m_widget = valueGroup;

    for (int i = 0; i < 3; ++i) {
        m_widget->connect<void(QDoubleSpinBox::*)(double)>(m_v[i], &QDoubleSpinBox::valueChanged, [this] (double) {
            onValueChanged(value());
        });
    }
}

Vector3Property::~Vector3Property()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *Vector3Property::ui()
{
    return m_widget;
}

void Vector3Property::setMinMax(o3d::Float min, o3d::Float max)
{
    m_v[0]->setMinimum(min);
    m_v[0]->setMaximum(max);

    m_v[1]->setMinimum(min);
    m_v[1]->setMaximum(max);

    m_v[2]->setMinimum(min);
    m_v[2]->setMaximum(max);
}

o3d::Vector3f Vector3Property::value() const
{
    return Vector3f(m_v[0]->value(), m_v[1]->value(), m_v[2]->value());
}

void Vector3Property::setValue(const o3d::Vector3f &v)
{
    for (int i= 0; i < 3; ++i) {
        m_v[i]->blockSignals(true);
        m_v[i]->setValue(v[i]);
        m_v[i]->blockSignals(false);
    }
}

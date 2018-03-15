/**
 * @brief Vector2 panel property abstract model.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <QDoubleSpinBox>

#include "o3d/studio/common/ui/property/vector2property.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


Vector2Property::Vector2Property(Panel *panel, const String &name, const o3d::String &label) :
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

    l->addWidget(pw);
    m_widget = valueGroup;

    for (int i = 0; i < 2; ++i) {
        m_widget->connect<void(QDoubleSpinBox::*)(double)>(m_v[i], &QDoubleSpinBox::valueChanged, [this] (double) {
            onValueChanged(value());
        });
    }
}

Vector2Property::~Vector2Property()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *Vector2Property::ui()
{
    return m_widget;
}

void Vector2Property::setMinMax(o3d::Float min, o3d::Float max)
{
    if (m_v[0]) {
        m_v[0]->setMinimum(min);
        m_v[0]->setMaximum(max);

        m_v[1]->setMinimum(min);
        m_v[1]->setMaximum(max);
    }
}

o3d::Vector2f Vector2Property::value() const
{
    if (m_v[0]) {
        return Vector2f(m_v[0]->value(), m_v[1]->value());
    } else {
        return o3d::Vector2f(0, 0);
    }
}

void Vector2Property::setValue(const o3d::Vector2f &v)
{
    for (int i= 0; i < 2; ++i) {
        m_v[i]->blockSignals(true);
        m_v[i]->setValue(v[i]);
        m_v[i]->blockSignals(false);
    }
}

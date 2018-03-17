/**
 * @brief Panel property with a single float.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @detail
 */

#include <QDoubleSpinBox>

#include "o3d/studio/common/ui/property/floatproperty.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


FloatProperty::FloatProperty(Panel *panel, const String &name, const o3d::String &label) :
    PanelProperty(panel, name, label),
    m_x(nullptr),
    m_minExl(0.f),
    m_maxExl(0.f)
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
    m_x = new QDoubleSpinBox();
    m_x->blockSignals(true);
    m_x->setRange(-1000000000, 1000000000);
    m_x->setDecimals(6);
    m_x->setSingleStep(0.1);
    m_x->setPrefix("x:");
    m_x->setValue(0);
    m_x->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_x->setMinimumWidth(75);
    m_x->setKeyboardTracking(false);
    m_x->blockSignals(false);
    pl->addWidget(m_x);

    l->addWidget(pw);
    m_widget = valueGroup;

    m_widget->connect<void(QDoubleSpinBox::*)(double)>(m_x, &QDoubleSpinBox::valueChanged, [this] (double) {
        Float v = value();
        if (v > m_minExl && v < m_maxExl) {
            // adjust to nearest
            v = o3d::abs(v - m_minExl) > o3d::abs(v - m_maxExl) ? m_maxExl : m_minExl;
        }

        m_x->setValue(v);
        onValueChanged(value());
    });
}

FloatProperty::~FloatProperty()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *FloatProperty::ui()
{
    return m_widget;
}

void FloatProperty::setMinMax(o3d::Float min, o3d::Float max)
{
    if (m_x) {
        m_x->setMinimum(min);
        m_x->setMaximum(max);
    }
}

void FloatProperty::setPrecision(o3d::Int32 dec)
{
    if (m_x) {
        m_x->setDecimals(dec);
    }
}

o3d::Float FloatProperty::value() const
{
    if (m_x) {
        return m_x->value();
    } else {
        return 0;
    }
}

void FloatProperty::setValue(o3d::Float v)
{
    if (m_minExl != m_maxExl) {
        if (v > m_minExl && v < m_maxExl) {
            // adjust to nearest
            v = o3d::abs(v - m_minExl) > o3d::abs(v - m_maxExl) ? m_maxExl : m_minExl;
        }
    }

    if (m_x) {
        m_x->blockSignals(true);
        m_x->setValue(v);
        m_x->blockSignals(false);
    }
}

void FloatProperty::setForbiddenExclusiveRange(o3d::Float minExcl, o3d::Float maxExcl)
{
    if (minExcl > maxExcl) {
        return;
    }

    m_minExl = minExcl;
    m_maxExl = maxExcl;
}

void FloatProperty::unsetForbiddenExclusiveRange()
{
    m_minExl = m_maxExl = 0.f;
}

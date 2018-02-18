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
    m_x(nullptr)
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
    m_x->setRange(-1000000000, 1000000000);
    m_x->setDecimals(6);
    m_x->setSingleStep(0.1);
    m_x->setPrefix("x:");
    m_x->setValue(0);
    m_x->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_x->setMinimumWidth(75);
    pl->addWidget(m_x);

    l->addWidget(pw);
    m_widget = valueGroup;
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

o3d::Float FloatProperty::value() const
{
    if (m_x) {
        return m_x->value();
    } else {
        return 0;
    }
}

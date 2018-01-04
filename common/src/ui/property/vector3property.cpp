/**
 * @brief Vector3 panel property abstract model.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-04
 * @details
 */

#include "o3d/studio/common/ui/property/vector3property.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

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
    QDoubleSpinBox *x = new QDoubleSpinBox();
    x->setRange(-1000000000, 1000000000);
    x->setDecimals(6);
    x->setSingleStep(0.1);
    x->setPrefix("x:");
    x->setValue(0);
    x->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    x->setMinimumWidth(75);
    pl->addWidget(x);

    // y
    QDoubleSpinBox *y = new QDoubleSpinBox();
    y->setRange(-1000000000, 1000000000);
    y->setDecimals(6);
    y->setSingleStep(0.1);
    y->setPrefix("y:");
    y->setValue(0);
    y->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    y->setMinimumWidth(75);
    pl->addWidget(y);

    // z
    QDoubleSpinBox *z = new QDoubleSpinBox();
    z->setRange(-1000000000, 1000000000);
    z->setDecimals(6);
    z->setSingleStep(0.1);
    z->setPrefix("z:");
    z->setValue(0);
    z->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    z->setMinimumWidth(75);
    pl->addWidget(z);

    l->addWidget(pw);
    m_widget = valueGroup;
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

o3d::Vector3f Vector3Property::value() const
{
    return o3d::Vector3f(0, 0, 0);  // @todo from ui
}

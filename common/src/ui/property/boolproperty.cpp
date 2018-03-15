/**
 * @brief Panel property with a bool float.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @detail
 */

#include <QCheckBox>

#include "o3d/studio/common/ui/property/boolproperty.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


BoolProperty::BoolProperty(Panel *panel, const String &name, const o3d::String &label) :
    PanelProperty(panel, name, label),
    m_b(nullptr)
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

    // checkbox
    m_b = new QCheckBox();
    m_b->blockSignals(true);
    m_b->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    m_b->setMinimumWidth(75);
    m_b->blockSignals(false);
    pl->addWidget(m_b);

    l->addWidget(pw);
    m_widget = valueGroup;

    m_widget->connect(m_b, &QCheckBox::stateChanged, [this] (int) {
        onValueChanged(value());
    });
}

BoolProperty::~BoolProperty()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *BoolProperty::ui()
{
    return m_widget;
}

o3d::Bool BoolProperty::value() const
{
    if (m_b) {
        return m_b->isChecked();
    } else {
        return 0;
    }
}

void BoolProperty::setValue(o3d::Bool checked)
{
    if (m_b) {
        m_b->blockSignals(true);
        m_b->setChecked(checked);
        m_b->blockSignals(false);
    }
}

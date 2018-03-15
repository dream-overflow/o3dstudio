/**
 * @brief Panel property with a drop down list of predefined values.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-15
 * @details
 */

#include <QComboBox>

#include "o3d/studio/common/ui/property/dropdownproperty.h"
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/ui/uiutils.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace o3d::studio::common;


DropDownProperty::DropDownProperty(Panel *panel, const String &name, const o3d::String &label) :
    PanelProperty(panel, name, label),
    m_cb(nullptr)
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

    // combo box
    m_cb = new QComboBox();
    pl->addWidget(m_cb);

    l->addWidget(pw);
    m_widget = valueGroup;

    m_widget->connect<void(QComboBox::*)(int)>(m_cb, &QComboBox::currentIndexChanged, [this] (int) {
        onValueChanged(value());
    });
}

DropDownProperty::~DropDownProperty()
{
//    if (m_widget) {
//        m_widget->deleteLater();
//    }
}

QWidget *DropDownProperty::ui()
{
    return m_widget;
}

void DropDownProperty::addValue(const o3d::String &label, o3d::Int32 value)
{
    m_cb->blockSignals(true);
    m_cb->addItem(toQString(label), QVariant(value));
    m_cb->blockSignals(false);
}

o3d::Int32 DropDownProperty::value() const
{
    if (m_cb) {
        return (Int32)m_cb->currentData().toInt();
    } else {
        return -1;
    }
}

void DropDownProperty::setValue(o3d::Int32 value)
{
    if (m_cb) {
        int index = m_cb->findData(QVariant(value));
        m_cb->blockSignals(true);
        m_cb->setCurrentIndex(index);
        m_cb->blockSignals(false);
    }
}

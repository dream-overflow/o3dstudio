/**
 * @brief Toolset to construct a panel with properties.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-21
 * @details
 */

#include "o3d/studio/common/ui/panelbuilder.h"
#include "o3d/studio/common/ui/uiutils.h"
#include "o3d/studio/common/ui/property/panelproperty.h"

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

using namespace o3d::studio::common;

PanelBuilder::PanelBuilder(Panel *panel, const o3d::String &label) :
    m_panel(panel)
{
    O3D_ASSERT(m_panel != nullptr);

    m_widget = new QWidget();
    QVBoxLayout *vl = new QVBoxLayout();
    m_widget->setLayout(vl);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(0, 0, 0, 1);
    hl->setSpacing(0);

    QPushButton *groupButton = new QPushButton(UiUtils::tintIcon(":/icons/arrow_drop_down_circle_black.svg"), "");
    groupButton->setFlat(True);
    groupButton->setStyleSheet("border-bottom: 0px;");
    groupButton->setMinimumSize(QSize(24, 24));
    groupButton->setMaximumSize(QSize(24, 24));
    groupButton->setToolTip("Collapse/Expand");
    hl->addWidget(groupButton, 0, Qt::AlignLeft);

    QLabel *groupLabel = new QLabel(toQString(label));
    groupLabel->setStyleSheet("border-bottom: 0px none");
    hl->addWidget(groupLabel, 1, Qt::AlignCenter);

    QPushButton *groupPropButton = new QPushButton(UiUtils::tintIcon(":/icons/settings_black.svg"), "");
    groupPropButton->setFlat(True);
    groupPropButton->setStyleSheet("border-bottom: 0px;");
    groupPropButton->setMinimumSize(QSize(24, 24));
    groupPropButton->setMaximumSize(QSize(24, 24));
    groupPropButton->setToolTip("Settings");
    hl->addWidget(groupPropButton);

    QWidget *groupHeader = new QWidget();
    groupHeader->setLayout(hl);
    groupHeader->setStyleSheet("border-bottom: 1px solid grey");

    m_widget->layout()->addWidget(groupHeader);

    m_widget->connect(groupButton, &QPushButton::clicked, [vl](bool) {
        QLayout *l = vl;
        for (int i = 1; i < l->count(); ++i) {
            if (l->itemAt(i)->widget()->isHidden()) {
                l->itemAt(i)->widget()->setVisible(True);
            } else {
                l->itemAt(i)->widget()->setVisible(False);
            }
        }
    });
}

PanelBuilder::~PanelBuilder()
{

}

QWidget *PanelBuilder::ui()
{
    return m_widget;
}

void PanelBuilder::addPanelProperty(PanelProperty *pp)
{
    if (!pp) {
        return;
    }

    QWidget *widget = pp->ui();
    m_widget->layout()->addWidget(widget);
}

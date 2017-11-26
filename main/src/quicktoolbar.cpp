/**
 * @brief Application main window quick left fixed toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#include "quicktoolbar.h"

using namespace o3d::studio::main;


QuickToolBar::QuickToolBar(QWidget *parent) :
    QToolBar(tr("Quick toolbar"), parent)
{
    setMovable(false);
    setFloatable(false);

    setupButtons();

    setWindowIcon(QIcon(":/icons/apps_black.svg"));
}

QuickToolBar::~QuickToolBar()
{

}

QToolBar* QuickToolBar::ui()
{
    return this;
}

o3d::String QuickToolBar::elementName() const
{
    return "o3s::main::quicktoolbar";
}

Qt::ToolBarArea QuickToolBar::toolBarArea() const
{
    return Qt::LeftToolBarArea;
}

void QuickToolBar::onHomeBtn()
{
    showHome();
}

void QuickToolBar::setupButtons()
{
    addAction(QIcon::fromTheme("go-home"), tr("Home"), this, SLOT(onHomeBtn()));
}

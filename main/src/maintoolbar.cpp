/**
 * @brief Application main window main toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#include "maintoolbar.h"
#include "mainwindow.h"

using namespace o3d::studio::main;


MainToolBar::MainToolBar(QWidget *parent) :
    QToolBar(tr("Main toolbar"), parent)
{
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);
    setupButtons();

    setWindowIcon(QIcon::fromTheme("applications-accessories"));
}

MainToolBar::~MainToolBar()
{

}

QToolBar *MainToolBar::ui()
{
    return this;
}

QString MainToolBar::elementName() const
{
    return("o3s::main::maintoolbar");
}

Qt::ToolBarArea MainToolBar::toolBarArea() const
{
    return Qt::TopToolBarArea;
}

void MainToolBar::setupButtons()
{
   // addAction(QIcon::fromTheme("go-home"), tr("Home"), this, "onHomeBtn");
}

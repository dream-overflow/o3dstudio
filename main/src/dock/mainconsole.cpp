/**
 * @brief Application main console dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-31
 * @details
 */

#include "mainconsole.h"
#include "../mainwindow.h"

using namespace o3d::studio::main;


MainConsole::MainConsole(QWidget *parent) :
    QDockWidget(tr("Main console"), parent),
    common::Dock()
{
    setMinimumWidth(150);
    setMinimumHeight(150);

    setupUi();
}

MainConsole::~MainConsole()
{

}

QDockWidget *MainConsole::ui()
{
    return this;
}

QString MainConsole::elementName() const
{
    return("o3s::main::mainconsole");
}

Qt::DockWidgetArea MainConsole::dockWidgetArea() const
{
    return Qt::BottomDockWidgetArea;
}

void MainConsole::setupUi()
{

}

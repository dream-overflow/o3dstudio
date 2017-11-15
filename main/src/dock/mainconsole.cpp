/**
 * @brief Application main console dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-31
 * @details
 */

#include "mainconsole.h"
#include "../mainwindow.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::main;


MainConsole::MainConsole(QWidget *parent) :
    QDockWidget(tr("Main console"), parent),
    common::Dock(),
    m_listWidget(nullptr)
{
    setMinimumWidth(150);
    setMinimumHeight(150);

    setupUi();

    common::Messenger& messenger = common::Application::instance()->messenger();
    connect(&messenger, SIGNAL(onNewMessage(QtMsgType, const QString&)), SLOT(onMessage(QtMsgType, const QString&)));
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

void MainConsole::onMessage(QtMsgType msgType, const QString &message)
{
    QIcon icon = QIcon::fromTheme("dialog-question");

    switch (msgType) {
        case QtDebugMsg:
            icon = QIcon::fromTheme("edit-delete");
            break;
        case QtWarningMsg:
            icon = QIcon::fromTheme("emblem-important");
            break;
        case QtCriticalMsg:
            icon = QIcon::fromTheme("process-stop");
            break;
        case QtFatalMsg:
            icon = QIcon::fromTheme("emblem-unreadable");
            break;
        case QtInfoMsg:
            icon = QIcon::fromTheme("dialog-information");
            break;
        default:
            break;
    }

    QListWidgetItem *item = new QListWidgetItem();

    item->setText(message);
    item->setIcon(icon);
    item->setToolTip(message);

    m_listWidget->addItem(item);
}

void MainConsole::setupUi()
{
    // @todo add a layout with some filters and a auto scroll button
    m_listWidget = new QListWidget();
    m_listWidget->setAutoScroll(true);
    m_listWidget->setIconSize(QSize(16, 16));
    setWidget(m_listWidget);
}

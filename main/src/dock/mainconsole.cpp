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


QtMainConsole::QtMainConsole(QWidget *parent) :
    QDockWidget(tr("Main console"), parent),
    m_listWidget(nullptr)
{
    setMinimumWidth(150);
    setMinimumHeight(150);

    setupUi();
}

QtMainConsole::~QtMainConsole()
{

}

void QtMainConsole::setupUi()
{
    // @todo add a layout with some filters and a auto scroll button
    m_listWidget = new QListWidget();
    m_listWidget->setAutoScroll(true);
    m_listWidget->setIconSize(QSize(16, 16));
    setWidget(m_listWidget);
    setWindowIcon(QIcon(":/icons/perm_device_information_black.svg"));
}

MainConsole::MainConsole(o3d::BaseObject *parent) :
    BaseObject(parent),
    m_qtMainConsole(nullptr)
{
    m_qtMainConsole = new QtMainConsole();

    common::Messenger& messenger = common::Application::instance()->messenger();
    messenger.onNewMessage.connect(this, &MainConsole::onMessage, CONNECTION_ASYNCH);
}

MainConsole::~MainConsole()
{
    // deletePtr(m_qtMainConsole);
}

QDockWidget *MainConsole::ui()
{
    return m_qtMainConsole;
}

o3d::String MainConsole::elementName() const
{
    return "o3s::main::mainconsole";
}

Qt::DockWidgetArea MainConsole::dockWidgetArea() const
{
    return Qt::BottomDockWidgetArea;
}

void MainConsole::onMessage(UInt32 msgType, String message)
{
    QIcon icon = QIcon::fromTheme("dialog-question");

    if (msgType >= common::Messenger::STATUS_MSG) {
        return;
    }

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

    item->setText(toQString(message));
    item->setIcon(icon);
    item->setToolTip(toQString(message));

    m_qtMainConsole->m_listWidget->addItem(item);

    if (m_qtMainConsole->m_listWidget->count() > MAX_NUM_MESSAGES) {
        m_qtMainConsole->m_listWidget->takeItem(0);
    }

    m_qtMainConsole->m_listWidget->scrollToBottom();
}

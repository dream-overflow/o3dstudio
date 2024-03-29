/**
 * @brief Application main entry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <QtWidgets/QApplication>

#include <QtCore/QResource>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QTime>

#include "mainwindow.h"
#include "settings.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/messenger.h"

#include <o3d/core/filemanager.h>
#include <o3d/core/memorymanager.h>
#include <o3d/core/application.h>
#include <o3d/core/debug.h>
#include <o3d/core/timer.h>
#include <o3d/core/callback.h>
#include <o3d/core/evtmanager.h>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void o3sMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    QHash<QtMsgType, QString> msgLevelHash({
        {QtDebugMsg, "Debug"},
        {QtInfoMsg, "Info"},
        {QtWarningMsg, "Warning"},
        {QtCriticalMsg, "Critical"},
        {QtFatalMsg, "Fatal"}
    });

    QTime time = QTime::currentTime();
    QByteArray localMsg = msg.toLocal8Bit();
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QString logLevelName = msgLevelHash[type];
    QByteArray logLevelMsg = logLevelName.toLocal8Bit();

    QFile outFile(o3d::studio::main::MAIN_LOG_FILE);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);

    QString txt = QString("%1 %2: %3 (%4)").arg(formattedTime, logLevelName, msg,  context.file);
    ts << txt << endl;

    // @todo options
    // and to stderr
    fprintf(stderr, "%s %s: %s (%s:%u, %s)\n",
            formattedTimeMsg.constData(),
            logLevelMsg.constData(),
            localMsg.constData(),
            context.file,
            context.line,
            context.function);
    fflush(stderr);

    if (o3d::studio::common::Application::hasInstance()) {
        o3d::studio::common::Messenger& messenger = o3d::studio::common::Application::instance()->messenger();
        messenger.message(type, fromQString(msg));
    }

    // no default handler
    // (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

void onExit()
{
    // terminate cleanly if the program call exit before an Application::quit().
    if (o3d::Application::isInit()) {
        o3d::Application::quit();
    }
}

// static o3d::AppWindow *s_mainWindowPtr = nullptr;
static o3d::studio::main::MainWindow *s_mainWindowPtr = nullptr;

o3d::Int32 processEvtManager(void *) {
    QEvent *event = new QEvent((QEvent::Type)(QEvent::User+1));
    if (s_mainWindowPtr) {
        QApplication::postEvent(s_mainWindowPtr->mainWindow(), event, Qt::HighEventPriority);
    }
    return 0;
}

o3d::Int32 mainTimer(void *data) {
    // o3d::System::print(o3d::String::print("1> %i", o3d::System::getMsTime()), "");
    o3d::Application::run(o3d::True);
//    if (o3d::EvtManager::instance()->isPendingEvent()) {
//        o3d::EvtManager::instance()->processEvent();
//    }

    return 0;
}

o3d::Int32 threadedTimer(void *data) {
    // o3d::System::print(o3d::String::print("2> %i", o3d::System::getMsTime()), "");
    return 0;
}

int main(int argc, char *argv[])
{
    // Warn the debugger that we want to detect memory leaks
    #if defined(_DEBUG) && defined(Q_CC_MSVC)
        int lDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        lDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
//		lDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
//		lDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
        lDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(lDbgFlag);
    #endif

    atexit(onExit);

    // o3d initialization
    o3d::AppSettings settings;

    try {
        o3d::Application::init(settings, argc, argv);
        o3d::EvtManager::instance()->setWakeUpCallback(new o3d::CallbackFunction(processEvtManager));

        // cleared log out file with new header
        // o3d::Debug::instance()->setDefaultLog("objective3d.log");
        // o3d::Debug::instance()->getDefaultLog().clearLog();

        o3d::MemoryManager::instance()->enableLog(o3d::MemoryManager::MEM_RAM, 128);
        o3d::MemoryManager::instance()->enableLog(o3d::MemoryManager::MEM_GFX);
    } catch (o3d::E_BaseException &ex) {
        o3d::Application::message(
            o3d::String("Failed to initialize the application: ") + ex.what(),
            o3d::Application::getAppName(),
            o3d::Application::ICON_ERROR);

        return -1;
    }

    int lExitcode = 0;

    qInstallMessageHandler(o3sMessageHandler);

    QApplication lApp(argc, argv);

    QCoreApplication::setOrganizationName("Dream Overflow");
    QCoreApplication::setApplicationName("Objective-3D Studio");

    o3d::FileManager::instance()->setPackExt("*.zip|*.dat|*.rc");
    o3d::FileManager::instance()->mountAsset("zip://", "o3smainresources.zip");
    // o3d::FileManager::instance()->mountAsset("zip://", "o3scommonresource.zip");

    o3d::studio::common::Application::instance()->start();

    o3d::studio::main::MainWindow *lMainWindow = new o3d::studio::main::MainWindow();
    s_mainWindowPtr = lMainWindow;

    lMainWindow->mainWindow()->show();
//    lMainWindow->create(
//                800,
//                600,
//                o3d::AppWindow::COLOR_RGBA8,
//                o3d::AppWindow::DEPTH_24_STENCIL_8,
//                o3d::AppWindow::NO_MSAA,
//                o3d::False,
//                o3d::True);

    o3d::studio::common::Application::instance()->setupUi();

    // start timer once main windows is ready to receive events
    new o3d::Timer(50, o3d::Timer::TIMER_TIMEOUT, new o3d::CallbackFunction(mainTimer));
    // new o3d::TimerThread(16, o3d::Timer::TIMER_TIMEOUT, new o3d::CallbackFunction(threadedTimer));

    try {
        lExitcode = lApp.exec();
    } catch (o3d::E_BaseException &ex) {
        o3d::Application::message(
            o3d::String("Exception not caught during application execution: ") + ex.what(),
            o3d::Application::getAppName(),
            o3d::Application::ICON_ERROR);
    }

    // process remaining events
    o3d::EvtManager::instance()->processEvent();

    o3d::deletePtr(lMainWindow);
    s_mainWindowPtr = nullptr;

    o3d::studio::common::Application::instance()->stop();
    o3d::studio::common::Application::destroy();

    o3d::Application::quit();

    return lExitcode;
}

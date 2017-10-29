/**
 * @brief Application main entry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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
#include "common/application.h"

QT_BEGIN_NAMESPACE
// Q_IMPORT_PLUGIN(o3spluginoff)
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

    // no default handler
    // (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
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

    qInstallMessageHandler(o3sMessageHandler);

    QApplication lApp(argc, argv);

    QCoreApplication::setOrganizationName("Dream Overflow");
    QCoreApplication::setApplicationName("Objective-3D Studio");

    // QResource::registerResource("mainresources.rcc");
    // QResource::registerResource("darkorangetheme.rcc");

    o3d::studio::common::Application::instance()->start();

    o3d::studio::main::MainWindow *lMainWindow = new o3d::studio::main::MainWindow();
    lMainWindow->show();

    int lResult = lApp.exec();

    delete lMainWindow;
    lMainWindow = nullptr;

    // QResource::unregisterResource("darkorangetheme.rcc");
    // QResource::unregisterResource("mainresources.rcc");

    o3d::studio::common::Application::instance()->stop();
    o3d::studio::common::Application::destroy();

    return lResult;
}

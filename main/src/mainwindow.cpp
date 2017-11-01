/**
 * @brief Application main window
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-17
 * @details
 */

#include "mainwindow.h"
#include "settings.h"

#include "common/settings.h"

#include "common/command/commandmanager.h"
#include "common/command/dummycommand.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtGui/QCloseEvent>

#include "preferencesdialog.h"
#include "pluginsinformationsdialog.h"
#include "manageworkspacesdialog.h"
#include "newprojectdialog.h"
//#include "newresourcedialog.h"

#include "ui_aboutdialog.h"
#include "ui_systeminfodialog.h"

#include "common/modulemanager.h"
#include "common/application.h"
#include "common/workspace/workspacemanager.h"
#include "common/workspace/workspace.h"
#include "common/ui/uicontroller.h"
#include "common/ui/canvas/o3dcanvascontent.h"

#include "maintoolbar.h"
#include "quicktoolbar.h"

#include "dock/mainconsole.h"
// #include "dock/workspace.h"

#include "content/browsercontent.h"

#include <o3d/core/base.h>
#include <o3d/core/thread.h>
#include <o3d/core/evtmanager.h>

using namespace o3d::studio::main;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui()
{
    statusBar()->showMessage(tr("Objective-3D Studio starting..."));

    ui.setupUi(this);

    common::UiController &uiCtrl = common::Application::instance()->ui();

    connect(ui.actionNewProject, SIGNAL(triggered(bool)), SLOT(onFileNewProject()));

    connect(ui.actionNewResource, SIGNAL(triggered(bool)), SLOT(onFileNewResource()));
    ui.actionNewResource->setEnabled(false);

    connect(ui.actionWorkspaceManage, SIGNAL(triggered(bool)), SLOT(onFileWorkspaceManage()));

    connect(ui.actionPreferences, SIGNAL(triggered(bool)), SLOT(onFileMenuPreferences()));
    connect(ui.actionClose, SIGNAL(triggered(bool)), SLOT(onFileMenuClose()));
    ui.actionClose->setEnabled(false);

    connect(ui.actionQuit, SIGNAL(triggered(bool)), SLOT(onFileMenuQuit()));

    connect(ui.actionWindowFullscreen, SIGNAL(triggered(bool)), SLOT(onWindowFullScreen()));
    connect(ui.actionViewHomePage, SIGNAL(triggered(bool)), SLOT(onViewHomePage()));

    connect(ui.actionHelpIndex, SIGNAL(triggered(bool)), SLOT(onHelpIndex()));
    connect(ui.actionSystemInfo, SIGNAL(triggered(bool)), SLOT(onSystemInfo()));
    connect(ui.actionAboutPlugins, SIGNAL(triggered(bool)), SLOT(onAboutPlugin()));
    connect(ui.actionAbout, SIGNAL(triggered(bool)), SLOT(onAbout()));

    common::UiController &uiController = common::Application::instance()->ui();

    // connections to ui controller
    connect(&uiController, SIGNAL(attachContent(QString, QWidget*)), SLOT(onAttachContent(QString, QWidget*)));
    connect(&uiController, SIGNAL(attachDock(QString, QDockWidget*, Qt::DockWidgetArea)), SLOT(onAttachDock(QString, QDockWidget*, Qt::DockWidgetArea)));
    connect(&uiController, SIGNAL(attachToolBar(QString, QToolBar*, Qt::ToolBarArea)), SLOT(onAttachToolBar(QString, QToolBar*, Qt::ToolBarArea)));

    connect(&uiController, SIGNAL(detachContent(QString, QWidget*)), SLOT(onDetachContent(QString, QWidget*)));
    connect(&uiController, SIGNAL(detachDock(QString, QDockWidget*)), SLOT(onDetachDock(QString, QDockWidget*)));
    connect(&uiController, SIGNAL(detachToolBar(QString, QToolBar*)), SLOT(onDetachToolBar(QString, QToolBar*)));

    connect(&uiController, SIGNAL(showContent(QString, QWidget*, bool)), SLOT(onShowContent(QString, QWidget*, bool)));

    // connections to settings controller
    connect(&common::Application::instance()->settings(),
            SIGNAL(settingChanged(const QString &, const QVariant &)),
            SLOT(onSettingChanged(const QString &, const QVariant &)));

    // connections to command manager
    connect(&common::Application::instance()->command(),
            SIGNAL(commandDone(QString, QString, bool)), SLOT(onCommandDone(QString, QString, bool)));

    // connections to menu edit
    connect(ui.actionUndo, SIGNAL(triggered()), SLOT(onUndoAction()));
    connect(ui.actionRedo, SIGNAL(triggered()), SLOT(onRedoAction()));

    applySettings();

    // main toolbar
    MainToolBar *mainToolBar = new MainToolBar();
    mainToolBar->addWidget(new QLabel(tr("Search")));
    mainToolBar->addWidget(new QLineEdit());
    uiCtrl.addToolBar(mainToolBar);

    // quick toolbar
    QuickToolBar *quickToolBar = new QuickToolBar();
    uiCtrl.addToolBar(quickToolBar);

    connect(quickToolBar, SIGNAL(showHome()), SLOT(onViewHomePage()));

    // workspace dock @todo using Dock
    createDock(tr("Workspace"), "workspace", Qt::LeftDockWidgetArea);

    // property dock @todo using Dock
    createDock(tr("Property"), "property", Qt::RightDockWidgetArea);

    // main console dock
    MainConsole *mainConsole= new MainConsole();
    uiCtrl.addDock(mainConsole);

    // @todo need a custom title bar
    // consoleDock->titleBarWidget()->addAction(new QAction(QIcon::fromTheme("go-previous"), "Collapse"));

    // welcome central
    BrowserContent *browserContent = new BrowserContent();
    uiCtrl.addContent(browserContent);
    uiCtrl.setActiveContent(browserContent, true);

    // @todo setup status bar with some fixed widgets addPermanentWidget()
    statusBar()->showMessage(tr("Objective-3D Studio successfully started !"));

    // for test execute a dummy command @todo remove me
    common::Application::instance()->command().addCommand(new common::DummyCommand());

    // process o3d events
    // @todo should look using post events http://doc.qt.io/qt-5/qabstracteventdispatcher.html
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateObjective3D()));
    timer->start(5);
}

MainWindow::~MainWindow()
{
    commitSettings();

    common::UiController &uiCtrl = common::Application::instance()->ui();
    uiCtrl.disconnect(this);

    // docks
    for (auto it = m_docks.begin(); it != m_docks.end(); ++it) {
        if (!uiCtrl.removeDock(it.key())) {
            it.value()->setParent(nullptr);
        }
        delete it.value();
    }

    // toolbars
    for (auto it = m_toolBars.begin(); it != m_toolBars.end(); ++it) {
        if (!uiCtrl.removeToolBar(it.key())) {
            it.value()->setParent(nullptr);
        }
        delete it.value();
    }

    // contents
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it) {
        if (!uiCtrl.removeContent(it.key())) {
            it.value()->setParent(nullptr);
        }
        delete it.value();
    }
}

QDockWidget *MainWindow::createDock(const QString &title, const QString &name, Qt::DockWidgetArea area)
{
    if (m_docks.contains(name)) {
        return nullptr;
    }

    QDockWidget *dock = new QDockWidget(title);
    addDockWidget(area, dock);
    dock->setMinimumWidth(200);
    dock->setMinimumHeight(200);
    dock->setProperty("name", QVariant(name));
    m_docks.insert(dock->property("name").toString(), dock);

    return dock;
}

bool MainWindow::setupDock(const QString &name, QDockWidget *dock, Qt::DockWidgetArea area)
{
    if (m_docks.contains(name)) {
        return false;
    }

    // Qt::Orientation orientation ?
    addDockWidget(area, dock);
    dock->setMinimumWidth(200);
    dock->setMinimumHeight(200);
    dock->setProperty("name", QVariant(name));
    m_docks.insert(dock->property("name").toString(), dock);

    return true;
}

QToolBar *MainWindow::createToolBar(const QString &title, const QString &name, Qt::ToolBarArea area)
{
    if (m_toolBars.contains(name)) {
        return nullptr;
    }

    QToolBar *toolBar = new QToolBar(title);
    addToolBar(area, toolBar);
    toolBar->setMinimumWidth(48);
    toolBar->setMinimumHeight(48);
    toolBar->setProperty("name", QVariant(name));
    m_toolBars.insert(toolBar->property("name").toString(), toolBar);

    return toolBar;
}

bool MainWindow::setupToolBar(const QString &name, QToolBar *toolBar, Qt::ToolBarArea area)
{
    if (m_toolBars.contains(name)) {
        return false;
    }

    addToolBar(area, toolBar);
    toolBar->setMinimumWidth(48);
    toolBar->setMinimumHeight(48);
    toolBar->setProperty("name", QVariant(name));
    m_toolBars.insert(toolBar->property("name").toString(), toolBar);

    return true;
}

bool MainWindow::addContentWidget(const QString &name, QWidget *widget)
{
    if (m_contents.contains(name)) {
        return false;
    }

    m_contents.insert(name, widget);

    return true;
}

bool MainWindow::removeContentWidget(const QString &name)
{
    // cannot remove the welcome content widget
    if (name == "welcome") {
        return false;
    }

    // @todo using ctrl

    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        if (m_currentContent && m_currentContent == it.value()) {
            m_currentContent->setParent(nullptr);
            m_currentContent = nullptr;

            setCurrentContentWidget("welcome");
        }

        it.value()->setParent(nullptr);
        delete it.value();

        m_contents.erase(it);

        return true;
    }

    return false;
}

bool MainWindow::setCurrentContentWidget(const QString &name)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        // already current
        if (m_currentContent && m_currentContent == it.value()) {
            return true;
        }

        // unparent previous
        if (m_currentContent && m_currentContent != it.value()) {
            m_currentContent->setParent(nullptr);
        }

        // setup
        setCentralWidget(it.value());
        m_currentContent = it.value();

        return true;
    }

    return false;
}

QWidget *MainWindow::contentWidget(const QString &name)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        return it.value();
    } else {
        return nullptr;
    }
}

const QWidget *MainWindow::contentWidget(const QString &name) const
{
    auto cit = m_contents.find(name);
    if (cit != m_contents.cend()) {
        return cit.value();
    } else {
        return nullptr;
    }
}

const QString& MainWindow::language() const
{
    return m_currentLanguage;
}

const QString& MainWindow::theme() const
{
    return m_currentTheme;
}

/*void MainWindow::loadPlugin()
    o3d::studio::common::GuiPluginInterface * lpInterface = nullptr;

    // Loading of static plugins
    foreach (QObject *lpPlugin, QPluginLoader::staticInstances())
    {
        lpInterface = qobject_cast<o3scommon::GuiPluginInterface *>(lpPlugin);

        if (lpInterface != NULL)
            m_modules.push_back(lpInterface);
    }

    // Loading of dynamic plugins
    const QStringList lModulePathList = ModulePreferences::GetModulePathList();

    foreach (QString lPath, lModulePathList)
    {
        QDir lPluginsDir(lPath);

        foreach (QString lFileName, lPluginsDir.entryList(QStringList(O3S_MODULE_EXT), QDir::Files))
        {
            QPluginLoader lLoader(lPluginsDir.absoluteFilePath(lFileName));

            lpInterface = qobject_cast<o3scommon::GuiPluginInterface *>(lLoader.instance());

            if (lpInterface != NULL)
                m_modules.push_back(lpInterface);
        }
    }

    // We append now each plugin into the menu "Start"
    QList<QAction*> lActionToAdd;

    foreach(o3scommon::GuiPluginInterface * lpInterface, m_modules)
    {
        if (lpInterface != NULL)
        {
            if (!lpInterface->GetMenuItemName().isEmpty())
            {
                QAction * lpAction = new QAction(menuStart);
                lpAction->setProperty("ModuleName", QVariant(lpInterface->GetModuleName()));
                lpAction->setText(lpInterface->GetMenuItemName());
                lpAction->setToolTip(lpInterface->GetMenuItemTooltip());
                lpAction->setIcon(lpInterface->GetMenuItemIcon());

                connect(lpAction, SIGNAL(triggered(bool)), SLOT(OnStartMenuModuleActivated()));

                lActionToAdd.push_back(lpAction);
            }
        }
    }

    menuStart->insertActions(menuStart->actions().front(), lActionToAdd);

    // We load the default module
    QString lStartModule = ModulePreferences::GetAutoStartModuleName();

    if (!lStartModule.isEmpty()) {
        startModule(lStartModule);
    } else {
        startModule("Default");
    }
}
*/
bool MainWindow::applySettings()
{
    common::Settings &settings = common::Application::instance()->settings();

    resize(settings.get("o3s::main::window::size", QVariant(QSize(800, 400))).toSize());
    move(settings.get("o3s::main::window::position", QVariant(QPoint(100, 100))).toPoint());

    loadLanguage(settings.get("o3s::main::language", QVariant("default")).toString());
    setThemeColor(settings.get("o3s::main::theme::color", QVariant("dark")).toString());

    return true;
}

bool MainWindow::commitSettings()
{
    common::Settings &settings = common::Application::instance()->settings();

    settings.set("o3s::main::window::size", QVariant(size()));
    settings.set("o3s::main::window::position", QVariant(pos()));

    return true;
}

bool MainWindow::setThemeColor(const QString &theme)
{
    if (theme == m_currentTheme) {
        return true;
    }

    if (theme == "dark") {
    /*    qApp->setStyle(QStyleFactory::create("dark"));

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(53,53,53));   // and menu background
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        QPalette lightPalette = qApp->palette();

        qApp->setPalette(darkPalette);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
*/
        QFile themeFile(":/style/darktheme.qss");
        themeFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray content = themeFile.readAll();

        themeFile.close();

        qApp->setStyle(QStyleFactory::create("dark"));  // "plastique");
        qApp->setStyleSheet(content);

        m_currentTheme = theme;
        return true;
    } else if (theme == "light") {
        qApp->setStyle(QStyleFactory::create("light"));

        qApp->setPalette(style()->standardPalette());
        qApp->setStyleSheet("");

        menuBar()->setPalette(style()->standardPalette());
        menuBar()->setStyleSheet("");

        m_currentTheme = theme;
        return true;
    } else if (theme == "darkorange") {
        QFile themeFile(":/style/darkorangetheme.qss");
        themeFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray content = themeFile.readAll();

        themeFile.close();

        qApp->setStyle(QStyleFactory::create("darkorange"));  // "plastique");
        qApp->setStyleSheet(content);

        m_currentTheme = theme;
        return true;
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent* _pEvent)
{
    closeWorkspace();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event != nullptr) {
        switch(event->type()) {
            // this event is send if a translator is loaded
            case QEvent::LanguageChange:
                ui.retranslateUi(this);
                break;

            // this event is send, if the system, language changes
            case QEvent::LocaleChange:
            {
                // default to locale if possible
                if (common::Application::instance()->settings().get("o3s::main::language") == "default") {
                    QString locale = QLocale::system().name();
                    locale.truncate(locale.lastIndexOf('_'));
                    loadLanguage(locale);
                }
            }
                break;

            default:
                break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::updateObjective3D()
{
    if (o3d::EvtManager::instance()->isPendingEvent()) {
        o3d::EvtManager::instance()->processEvent();
    }
}

void MainWindow::onFileNewProject()
{
    // new project dialog
    NewProjectDialog *dialog = new NewProjectDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onFileNewResource()
{
    // new resource dialog
//    NewResourceDialog *dialog = new NewResourceDialog(this);
//    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
//    dialog->show();
}

void MainWindow::onFileMenuPreferences()
{
    // preferences dialog
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onFileMenuClose()
{
    closeWorkspace();
}

void MainWindow::onFileMenuQuit()
{
    common::Workspace *workspace = common::Application::instance()->workspaceManager().current();
    if (workspace) {
        // @todo if non-saved projects ask
        workspace->save();
    }

    close();
}

void MainWindow::onWindowFullScreen()
{
    if (!isFullScreen()) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::onFileWorkspaceManage()
{
    // workspace manager dialog
    ManageWorkspaceDialog *dialog = new ManageWorkspaceDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onViewHomePage()
{
    // browser
    common::UiController &uiCtrl = common::Application::instance()->ui();
    BrowserContent *browserContent = static_cast<BrowserContent*>(uiCtrl.content("o3s::main::browsercontent"));

    if (browserContent) {
        browserContent->setSource(QUrl::fromLocalFile("index.html"));
        uiCtrl.setActiveContent(browserContent, true);
    }
}

void MainWindow::onHelpIndex()
{
    // browser
    common::UiController &uiCtrl = common::Application::instance()->ui();
    BrowserContent *browserContent = static_cast<BrowserContent*>(uiCtrl.content("o3s::main::browsercontent"));

    if (browserContent) {
        browserContent->setSource(QUrl::fromLocalFile("help/index.html"));
        uiCtrl.setActiveContent(browserContent, true);
    }
}

void MainWindow::onSystemInfo()
{
    // system info dialog
    QDialog *dialog = new QDialog(this);
    Ui::SystemInfoDialog ui;
    ui.setupUi(dialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onAboutPlugin()
{
    // preferences dialog
    PluginsInformationsDialog *dialog = new PluginsInformationsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onAbout()
{
    // about dialog
    QDialog *dialog = new QDialog(this);
    Ui::AboutDialog ui;
    ui.setupUi(dialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void MainWindow::onSettingChanged(const QString &key, const QVariant &value)
{
    if (key == "o3s::main::language") {
        QString language = value.toString();
        loadLanguage(language);

        QLocale locale = QLocale::system();

        if (language != "default") {
            locale = QLocale(language);
        }

        QString languageName = QLocale::languageToString(locale.language());
        statusBar()->showMessage(tr("Current language changed to %1").arg(languageName));
    } else if (key == "o3s::main::theme::color") {
        setThemeColor(value.toString());
        statusBar()->showMessage(tr("Current theme changed to %1").arg(m_currentTheme));
    }
}

void MainWindow::onAttachContent(QString name, QWidget *content)
{
    addContentWidget(name, content);
}

void MainWindow::onAttachDock(QString name, QDockWidget *dock, Qt::DockWidgetArea area)
{
    setupDock(name, dock, area);
}

void MainWindow::onAttachToolBar(QString name, QToolBar *toolBar, Qt::ToolBarArea area)
{
    setupToolBar(name, toolBar, area);
}

void MainWindow::onDetachContent(QString name, QWidget *content)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        removeContentWidget(name);
    }
}

void MainWindow::onDetachDock(QString name, QDockWidget *dock)
{
    auto it = m_docks.find(name);
    if (it != m_docks.end()) {
        m_docks.erase(it);
        removeDockWidget(dock);
    }
}

void MainWindow::onDetachToolBar(QString name, QToolBar *toolBar)
{
    auto it = m_toolBars.find(name);
    if (it != m_toolBars.end()) {
        m_toolBars.erase(it);
        removeToolBar(toolBar);
    }
}

void MainWindow::onUndoAction()
{
    common::Application::instance()->command().undoLastCommand();
}

void MainWindow::onRedoAction()
{
    common::Application::instance()->command().redoLastCommand();
}

void MainWindow::onShowContent(QString name, QWidget *content, bool showHide)
{
    if (content) {
        if (showHide) {
            setCurrentContentWidget(name);
        } else {
            setCentralWidget(nullptr);
        }
    }
}

void MainWindow::onCommandDone(QString name, QString label, bool done)
{
    if (done) {
        statusBar()->showMessage(tr("%1 done").arg(label));
        ui.actionUndo->setText(tr("Undo %1").arg(label));
    } else {
        statusBar()->showMessage(tr("%1 undone").arg(label));
        ui.actionRedo->setText(tr("Redo %1").arg(label));
    }

    if (common::Application::instance()->command().hasDoneCommands()) {
        ui.actionUndo->setEnabled(true);
    } else {
        ui.actionUndo->setEnabled(false);
    }

    if (common::Application::instance()->command().hasUndoneCommands()) {
        ui.actionRedo->setEnabled(true);
    } else {
        ui.actionRedo->setEnabled(false);
    }
}

void MainWindow::closeWorkspace()
{
    o3d::studio::common::Workspace *workspace = o3d::studio::common::Application::instance()->workspaceManager().current();
    if (workspace && workspace->hasChanges()) {
        statusBar()->showMessage(tr("Saving current workspace..."));
        workspace->save();
        statusBar()->showMessage(tr("Current workspace saved !"));
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    QString path = QDir::currentPath() + QDir::separator() + QString(o3d::studio::common::LANGUAGES_PATH);
    if (translator.load(filename, path, QString(), QString())) {
        qApp->installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString &language)
{
    common::Application::instance()->loadLanguage(language);

    if (m_currentLanguage != language) {
        QLocale locale = QLocale::system();
        QString languageCode = language;

        if (language != "default") {
            locale = QLocale(language);
        } else {
            languageCode = locale.name().split("_").at(0);
        }

        QString languageName = QLocale::languageToString(locale.language());

        switchTranslator(m_translator, QString("o3smain_%1.qm").arg(languageCode));
        m_currentLanguage = language;
    }
}

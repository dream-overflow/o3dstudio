/**
 * @brief Application main window
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-17
 * @details
 */

#include "mainwindow.h"
#include "settings.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTextBrowser>
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

#include "maintoolbar.h"
#include "quicktoolbar.h"

using namespace o3d::studio::main;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui()
{
    statusBar()->showMessage(tr("Objective-3D Studio starting..."));

    ui.setupUi(this);

    connect(ui.actionNewProject, SIGNAL(triggered(bool)), SLOT(onFileNewProject()));
    connect(ui.actionNewResource, SIGNAL(triggered(bool)), SLOT(onFileNewResource()));
    connect(ui.actionWorkspaceManage, SIGNAL(triggered(bool)), SLOT(onFileWorkspaceManage()));
    connect(ui.actionPreferences, SIGNAL(triggered(bool)), SLOT(onFileMenuPreferences()));
    connect(ui.actionClose, SIGNAL(triggered(bool)), SLOT(onFileMenuClose()));
    connect(ui.actionQuit, SIGNAL(triggered(bool)), SLOT(onFileMenuQuit()));

    connect(ui.actionWindowFullscreen, SIGNAL(triggered(bool)), SLOT(onWindowFullScreen()));
    connect(ui.actionViewHomePage, SIGNAL(triggered(bool)), SLOT(onViewHomePage()));

    connect(ui.actionHelpIndex, SIGNAL(triggered(bool)), SLOT(onHelpIndex()));
    connect(ui.actionSystemInfo, SIGNAL(triggered(bool)), SLOT(onSystemInfo()));
    connect(ui.actionAboutPlugins, SIGNAL(triggered(bool)), SLOT(onAboutPlugin()));
    connect(ui.actionAbout, SIGNAL(triggered(bool)), SLOT(onAbout()));

    applySettings();

    // main toolbar
    QToolBar *mainToolBar = new MainToolBar();
    setupToolBar("main", mainToolBar, Qt::TopToolBarArea);

    // quick toolbar
    QToolBar *quickToolBar = new QuickToolBar();
    setupToolBar("quick", quickToolBar, Qt::LeftToolBarArea);

    connect(quickToolBar, SIGNAL(showHome()), SLOT(onViewHomePage()));

    // @todo setup status bar with some fixed widgets addPermanentWidget()

    // workspace dock
    createDock(tr("Workspace"), "workspace", Qt::LeftDockWidgetArea);

    // property dock
    createDock(tr("Property"), "property", Qt::RightDockWidgetArea);

    // console dock
    QDockWidget *consoleDock = createDock(tr("Console"), "console", Qt::BottomDockWidgetArea);
    consoleDock->setMinimumWidth(150);
    consoleDock->setMinimumHeight(150);
    // @todo need a custom title bar
    // consoleDock->titleBarWidget()->addAction(new QAction(QIcon::fromTheme("go-previous"), "Collapse"));

    // welcome central
    QTextBrowser *welcome = new QTextBrowser();
    welcome->setSearchPaths(QStringList(common::Application::instance()->appDir() + QDir::separator() + "share/html"));
    welcome->setSource(QUrl::fromLocalFile("index.html"));

    addContentWidget("welcome", welcome);
    setCurrentContentWidget("welcome");

    statusBar()->showMessage(tr("Objective-3D Studio successfully started !"));
}

MainWindow::~MainWindow()
{
    commitSettings();

    // docks
    for (auto it = m_docks.begin(); it != m_docks.end(); ++it) {
        it.value()->setParent(nullptr);
        delete it.value();
    }

    // toolbars
    for (auto it = m_toolBars.begin(); it != m_toolBars.end(); ++it) {
        it.value()->setParent(nullptr);
        delete it.value();
    }

    // contents
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it) {
        if (it.value() == m_currentContent) {
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

QToolBar *MainWindow::createToolBar(const QString &title, const QString &name, Qt::ToolBarArea area)
{
    if (m_toolBars.contains(name)) {
        return nullptr;
    }

    QToolBar *toolBar = new QToolBar(title);
    addToolBar(area, toolBar);
    toolBar->setMinimumWidth(32);
    toolBar->setMinimumHeight(32);
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
    toolBar->setMinimumWidth(32);
    toolBar->setMinimumHeight(32);
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

bool MainWindow::closeCurrentModule()
{
    // If the module didnt clean the windows properly...
    if (centralWidget() != NULL)
        delete centralWidget();

    // We clean all remaining dock widget, toolbar, etc
    QObjectList lChildren = children();

    foreach (QObject * lpObject, lChildren)
    {
        QDockWidget * lpDock = qobject_cast<QDockWidget*>(lpObject);

        if (lpDock != NULL)
        {
            delete lpDock;
            continue;
        }

        QToolBar * lpToolBar = qobject_cast<QToolBar*>(lpObject);

        if (lpToolBar != NULL)
        {
            delete lpToolBar;
            continue;
        }
    }

    // We clean all the remaining menus (except the menu "Start")
    QObjectList lMenuBarChildren = menuBar()->children();

    foreach (QObject * lpObject, lMenuBarChildren)
    {
        QMenu * lpMenu = qobject_cast<QMenu*>(lpObject);

        if ((lpMenu != NULL) && (lpMenu != menuStart))
            delete lpMenu;
    }

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(new QVBoxLayout());
}*/

bool MainWindow::applySettings()
{
    const common::Settings &settings = common::Application::instance()->settings();

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
        qApp->setStyle(QStyleFactory::create("Dark"));

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

        m_currentTheme = theme;
        return true;
    } else if (theme == "light") {
        qApp->setStyle(QStyleFactory::create("Light"));

        qApp->setPalette(style()->standardPalette());
        qApp->setStyleSheet("");

        menuBar()->setPalette(style()->standardPalette());
        menuBar()->setStyleSheet("");

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

void MainWindow::onFileNewProject()
{
    // new project dialog
    NewProjectDialog *dialog = new NewProjectDialog(this);
    dialog->show();
}

void MainWindow::onFileNewResource()
{
    // new resource dialog
//    NewResourceDialog *dialog = new NewResourceDialog(this);
//    dialog->show();
}

void MainWindow::onFileMenuPreferences()
{
    // preferences dialog
    PreferencesDialog *dialog = new PreferencesDialog(this);
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
    dialog->show();
}

void MainWindow::onViewHomePage()
{
    // browser
    QTextBrowser *welcome = static_cast<QTextBrowser*>(contentWidget("welcome"));
    welcome->setSource(QUrl::fromLocalFile("index.html"));

    setCurrentContentWidget("welcome");
}

void MainWindow::onHelpIndex()
{
    // browser
    QTextBrowser *welcome = static_cast<QTextBrowser*>(contentWidget("welcome"));
    welcome->setSource(QUrl::fromLocalFile("help/index.html"));

    setCurrentContentWidget("welcome");
}

void MainWindow::onSystemInfo()
{
    // system info dialog
    QDialog *dialog = new QDialog(this);
    Ui::SystemInfoDialog ui;
    ui.setupUi(dialog);
    dialog->show();
}

void MainWindow::onAboutPlugin()
{
    // preferences dialog
    PluginsInformationsDialog *dialog = new PluginsInformationsDialog(this);
    dialog->show();
}

void MainWindow::onAbout()
{
    // about dialog
    QDialog *dialog = new QDialog(this);
    Ui::AboutDialog ui;
    ui.setupUi(dialog);
    dialog->show();
}

void MainWindow::closeWorkspace()
{
    o3d::studio::common::Workspace *workspace = o3d::studio::common::Application::instance()->workspaceManager().current();
    if (workspace && workspace->hasChanged()) {
        statusBar()->showMessage(tr("Objective-3D Studio saving current workspace..."));
        workspace->save();
        statusBar()->showMessage(tr("Objective-3D Studio current workspace saved !"));
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    QString path = QDir::currentPath() + QDir::separator() + QString(LANGUAGES_PATH);
    if (translator.load(filename, path, QString(), QString())) {
        qApp->installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString &language)
{
    if (m_currentLanguage != language) {
        QLocale locale = QLocale::system();
        QString languageCode = language;

        if (language != "default") {
            locale = QLocale(language);
        } else {
            languageCode = locale.name().split("_").at(0);
        }

        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        switchTranslator(m_translator, QString("o3smain_%1.qm").arg(languageCode));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(languageCode));

        // @todo and logger status bar message at certain levels
        statusBar()->showMessage(tr("Current language changed to %1").arg(languageName));

        m_currentLanguage = language;
    }
}

/**
 * @brief Application main window
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-17
 * @details
 */

#include "mainwindow.h"
#include "settings.h"

#include "o3d/studio/common/command/commandmanager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStackedWidget>
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

#include "o3d/studio/common/settings.h"
#include "o3d/studio/common/modulemanager.h"
#include "o3d/studio/common/application.h"
#include "o3d/studio/common/importer/importerregistry.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/ui/uicontroller.h"

#include "o3d/studio/common/ui/content.h"
#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/ui/toolbar.h"

#include "o3d/studio/common/ui/canvas/o3dcanvascontent.h"
#include "o3d/studio/common/ui/uiutils.h"

#include "maintoolbar.h"
#include "quicktoolbar.h"

#include "dock/mainconsole.h"
#include "dock/workspacedock.h"
#include "dock/propertydock.h"
#include "dock/selectiondock.h"

#include "content/browsercontent.h"

#include <o3d/core/base.h>
#include <o3d/core/thread.h>
#include <o3d/core/evtmanager.h>

using namespace o3d::studio::main;

QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui()
{
    // backup the original palette
    m_originalPalette = qApp->palette();

    ui.setupUi(this);

    m_stackedWidget = new QStackedWidget();
    setCentralWidget(m_stackedWidget);
}

void QtMainWindow::setup()
{
    // message
    common::Messenger& messenger = common::Application::instance()->messenger();

    // @todo setup status bar with some fixed widgets addPermanentWidget()
    messenger.info(fromQString(tr("Objective-3D Studio starting...")));

    // common gui @todo move to o3d
    common::UiController &uiCtrl = common::Application::instance()->ui();

    // menu actions
    connect(ui.actionNewProject, SIGNAL(triggered(bool)), SLOT(onFileNewProject()));
    connect(ui.actionOpenProject, SIGNAL(triggered(bool)), SLOT(onFileOpenProject()));
    connect(ui.actionSave, SIGNAL(triggered(bool)), SLOT(onFileMenuSave()));
    connect(ui.actionSaveAll, SIGNAL(triggered(bool)), SLOT(onFileMenuSaveAll()));
    connect(ui.actionSaveAs, SIGNAL(triggered(bool)), SLOT(onFileMenuSaveAs()));

    connect(ui.actionNewResource, SIGNAL(triggered(bool)), SLOT(onFileNewResource()));
    ui.actionNewResource->setEnabled(false);

    connect(ui.actionWorkspaceManage, SIGNAL(triggered(bool)), SLOT(onFileWorkspaceManage()));

    connect(ui.actionPreferences, SIGNAL(triggered(bool)), SLOT(onFileMenuPreferences()));

    connect(ui.actionImport, SIGNAL(triggered(bool)), SLOT(onFileMenuImport()));
    ui.actionImport->setEnabled(false);

    connect(ui.actionCloseProject, SIGNAL(triggered(bool)), SLOT(onFileMenuCloseProject()));
    ui.actionCloseProject->setEnabled(false);

    connect(ui.actionQuit, SIGNAL(triggered(bool)), SLOT(onFileMenuQuit()));

    connect(ui.actionWindowFullscreen, SIGNAL(triggered(bool)), SLOT(onWindowFullScreen()));

    connect(ui.actionPreviousContentView, SIGNAL(triggered(bool)), SLOT(onViewPreviousContentAction()));
    connect(ui.actionNextContentView, SIGNAL(triggered(bool)), SLOT(onViewNextContentAction()));

    connect(ui.actionHelpIndex, SIGNAL(triggered(bool)), SLOT(onHelpIndex()));
    connect(ui.actionSystemInfo, SIGNAL(triggered(bool)), SLOT(onSystemInfo()));
    connect(ui.actionAboutPlugins, SIGNAL(triggered(bool)), SLOT(onAboutPlugin()));
    connect(ui.actionAbout, SIGNAL(triggered(bool)), SLOT(onAbout()));

    // connections to menu edit @todo move to o3d
    connect(ui.actionUndo, SIGNAL(triggered()), SLOT(onUndoAction()));
    connect(ui.actionRedo, SIGNAL(triggered()), SLOT(onRedoAction()));

    // main toolbar
    MainToolBar *mainToolBar = new MainToolBar();
    uiCtrl.addToolBar(mainToolBar);

    // quick toolbar
    QuickToolBar *quickToolBar = new QuickToolBar();
    uiCtrl.addToolBar(quickToolBar);

    // @todo setActive view... plus context into the quicktoolbar
    connect(quickToolBar, SIGNAL(showHome()), SLOT(onViewHomePage()));

    // dock
    setDockNestingEnabled(true);
    setDockOptions(
                QMainWindow::AllowNestedDocks |
                QMainWindow::AllowTabbedDocks |
                QMainWindow::VerticalTabs |
                QMainWindow::GroupedDragging);

    // workspace dock
    WorkspaceDock *workspaceDock = new WorkspaceDock();
    uiCtrl.addDock(workspaceDock);

    // selection dock
    SelectionDock *selectionDock = new SelectionDock();
    uiCtrl.addDock(selectionDock);

    // property dock
    PropertyDock *propertyDock = new PropertyDock();
    uiCtrl.addDock(propertyDock);

    // main console dock
    MainConsole *mainConsole = new MainConsole();
    uiCtrl.addDock(mainConsole);

    // @todo need a custom title bar
    // consoleDock->titleBarWidget()->addAction(new QAction(QIcon::fromTheme("go-previous"), "Collapse"));

    // welcome central
    BrowserContent *browserContent = new BrowserContent();
    uiCtrl.addContent(browserContent);
    uiCtrl.setActiveContent(browserContent, true);

    // menu
    connect(ui.actionRecentsProjectsClearAll, SIGNAL(triggered(bool)), SLOT(onClearAllRecentProjects(bool)));
    connect(ui.actionRecentsResourcesClearAll, SIGNAL(triggered(bool)), SLOT(onClearAllRecentResources(bool)));

    initRecentProjectsMenu();
    initRecentResourcesMenu();

    messenger.info(fromQString(tr("Objective-3D Studio successfully started !")));

    // for test only execute a dummy command
    // common::Application::instance()->command().addCommand(new common::DummyCommand());
}

QtMainWindow::~QtMainWindow()
{

}

const o3d::String& QtMainWindow::language() const
{
    return m_currentLanguage;
}

const o3d::String& QtMainWindow::theme() const
{
    return m_currentTheme;
}

/*void MainWindow::loadPlugin()
    o3d::studio::common::GuiPluginInterface * lpInterface = nullptr;

    // Loading of static plugins
    foreach (QObject *lpPlugin, QPluginLoader::staticInstances())
    {
        lpInterface = qobject_cast<o3scommon::GuiPluginInterface *>(lpPlugin);

        if (lpInterface != nullptr)
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

            if (lpInterface != nullptr)
                m_modules.push_back(lpInterface);
        }
    }

    // We append now each plugin into the menu "Start"
    QList<QAction*> lActionToAdd;

    foreach(o3scommon::GuiPluginInterface * lpInterface, m_modules)
    {
        if (lpInterface != nullptr)
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

o3d::Bool QtMainWindow::setThemeColor(const String &theme)
{
    if (theme == m_currentTheme) {
        return True;
    }

    Bool changed = False;

    if (theme == "darkblue") {
        qApp->setStyle(QStyleFactory::create("darkblue"));

        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(53,53,53));   // and menu background
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));

        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(palette);

        QFile themeFile(":/style/darktheme.qss");
        themeFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray content = themeFile.readAll();

        themeFile.close();

        qApp->setStyle(QStyleFactory::create("darkblue"));  // "plastique");
        qApp->setStyleSheet(content);

        m_currentTheme = theme;
        changed = True;
    } else if (theme == "default") {
        qApp->setStyle(QStyleFactory::create("default"));

        qApp->setPalette(style()->standardPalette());
        qApp->setStyleSheet("");

        menuBar()->setPalette(style()->standardPalette());
        menuBar()->setStyleSheet("");

        m_currentTheme = theme;
        changed = True;
    } else if (theme == "darkorange") {
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(53,53,53));   // and menu background
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor("#ffa02f"));
        palette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(palette);

        QFile themeFile(":/style/darkorangetheme.qss");
        themeFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray content = themeFile.readAll();

        themeFile.close();

        qApp->setStyle(QStyleFactory::create("darkorange"));  // "plastique");
        qApp->setStyleSheet(content);

        m_currentTheme = theme;
        changed = True;
    }

    if (changed) {
        // for each menu
        for (QAction *menu: menuBar()->actions()) {
            if (menu->menu()) {
                common::UiUtils::tintMenu(menu->menu()->actions(), this);
            }
        }
    }

    return changed;
}

void QtMainWindow::closeEvent(QCloseEvent* _pEvent)
{
    Q_UNUSED(_pEvent)

    closeWorkspace();
}

void QtMainWindow::changeEvent(QEvent *event)
{
    common::UiController &uiCtrl = common::Application::instance()->ui();

    if (event != nullptr) {
        switch(event->type()) {
            // this event is send if a translator is loaded
            case QEvent::LanguageChange:
                ui.retranslateUi(this);

                // adapt window title @todo
                if (uiCtrl.activeContent() && !uiCtrl.activeContent()->ui()->windowTitle().isEmpty()) {
                    setWindowTitle(uiCtrl.activeContent()->ui()->windowTitle() + " - " + tr("Objective-3D Studio"));
                }

                break;

            // this event is send, if the system, language changes
            case QEvent::LocaleChange:
            {
                // default to locale if possible
                if (common::Application::instance()->settings().get("o3s::main::language") == "default") {
                    QString locale = QLocale::system().name();
                    locale.truncate(locale.lastIndexOf('_'));
                    loadLanguage(fromQString(locale));
                }
            }
                break;

            case QEvent::StyleChange:
                // @todo propagation for any concerned children
                break;

            case QEvent::PaletteChange:
                // @todo propagation for any concerned children
                break;

            default:
                break;
        }
    }
    QMainWindow::changeEvent(event);
}

void QtMainWindow::customEvent(QEvent *event)
{
    if (event->type() == QEvent::User+1) {
        o3d::EvtManager::instance()->processEvent();
    }
}

void QtMainWindow::onFileNewProject()
{
    // new project dialog
    NewProjectDialog *dialog = new NewProjectDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onFileOpenProject()
{
    QString dir = settings().get("o3s::main::project::previous-folder").toString();
    QString filename = QFileDialog::getOpenFileName(
                this, tr("Open project"), dir, "project.o3dstudio");

    if (!filename.isEmpty()) {
        QFileInfo lfile(filename);
        QDir ldir(lfile.absoluteDir());

        if (!ldir.exists()) {
            return;
        }

        openProject(ldir.absolutePath());

        if (ldir.cdUp()) {
            settings().set("o3s::main::project::previous-folder", ldir.absolutePath());
        }
    }
}

void QtMainWindow::onFileMenuSave()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace && workspace->activeProject()) {
        if (workspace->activeProject()->hasChanges()) {
            workspace->activeProject()->save();
        }
    }
}

void QtMainWindow::onFileMenuSaveAll()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        if (workspace->hasChanges()) {
            workspace->save();
        }
    }
}

void QtMainWindow::onFileMenuSaveAs()
{
    // @todo
}

void QtMainWindow::onFileNewResource()
{
    // new resource dialog
//    NewResourceDialog *dialog = new NewResourceDialog(this);
//    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
//    dialog->show();
}

void QtMainWindow::onFileMenuPreferences()
{
    // preferences dialog
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onFileMenuImport()
{
    // list of file exts from importers
    QString recognizedExts = toQString(common::Application::instance()->importers().supportedExts());

    QString dir = settings().get("o3s::main::import::previous-folder").toString();
    QString filename = QFileDialog::getOpenFileName(
                this, tr("Import file"), dir, recognizedExts);

    if (!filename.isEmpty()) {
        QFileInfo lfile(filename);
        QDir ldir(lfile.absoluteDir());

        if (!ldir.exists()) {
            return;
        }

        importFile(filename);

        settings().set("o3s::main::import::previous-folder", ldir.absolutePath());
    }
}

void QtMainWindow::onFileMenuCloseProject()
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            if (project->hasChanges()) {
                project->save();
            }

            workspace->closeProject(project->ref().light());
        }
    }
}

void QtMainWindow::onFileMenuQuit()
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        if (workspace->hasChanges()) {
            workspace->save();
        }

        common::Application::instance()->workspaces().deleteWorkspace(workspace->name());
    }

    close();
}

void QtMainWindow::onWindowFullScreen()
{
    if (!isFullScreen()) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void QtMainWindow::onFileWorkspaceManage()
{
    // workspace manager dialog
    ManageWorkspaceDialog *dialog = new ManageWorkspaceDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onViewHomePage()
{
    // browser
    common::UiController &uiCtrl = common::Application::instance()->ui();
    BrowserContent *browserContent = static_cast<BrowserContent*>(uiCtrl.content("o3s::main::browsercontent"));

    if (browserContent) {
        browserContent->setSource(QUrl::fromLocalFile("index.html"));
        uiCtrl.setActiveContent(browserContent, true);
    }
}

void QtMainWindow::onViewContent()
{
    QString name = sender()->property("name").toString();

    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::Content *content = uiCtrl.content(fromQString(name));

    if (content) {
        // special case for main browser
        if (name == "o3s::main::browsercontent") {
            BrowserContent *browserContent = static_cast<BrowserContent*>(content);
            browserContent->setSource(QUrl::fromLocalFile("index.html"));
        }

        uiCtrl.setActiveContent(content, true);
    }
}

void QtMainWindow::onViewDock()
{
    QString name = sender()->property("name").toString();

    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::Dock *dock = uiCtrl.dock(fromQString(name));

    if (dock) {
        if (!dock->ui()->isVisible()) {
            dock->ui()->show();
        } else {
            dock->ui()->hide();
        }
    }
}

void QtMainWindow::onViewToolBar()
{
    QString name = sender()->property("name").toString();

    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::ToolBar *toolBar = uiCtrl.toolBar(fromQString(name));

    if (toolBar) {
        if (!toolBar->ui()->isVisible()) {
            toolBar->ui()->show();
        } else {
            toolBar->ui()->hide();
        }
    }
}

void QtMainWindow::onHelpIndex()
{
    // browser
    common::UiController &uiCtrl = common::Application::instance()->ui();
    BrowserContent *browserContent = static_cast<BrowserContent*>(uiCtrl.content("o3s::main::browsercontent"));

    if (browserContent) {
        browserContent->setSource(QUrl::fromLocalFile("help/index.html"));
        uiCtrl.setActiveContent(browserContent, true);
    }
}

void QtMainWindow::onSystemInfo()
{
    // system info dialog
    QDialog *dialog = new QDialog(this);
    Ui::SystemInfoDialog ui;
    ui.setupUi(dialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onAboutPlugin()
{
    // preferences dialog
    PluginsInformationsDialog *dialog = new PluginsInformationsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onAbout()
{
    // about dialog
    QDialog *dialog = new QDialog(this);
    Ui::AboutDialog ui;
    ui.setupUi(dialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
}

void QtMainWindow::onUndoAction()
{
    common::Application::instance()->command().undoLastCommand();
}

void QtMainWindow::onRedoAction()
{
    common::Application::instance()->command().redoLastCommand();
}

void QtMainWindow::onViewPreviousContentAction()
{
    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::Content *content = uiCtrl.previousContent();

    if (content) {
        uiCtrl.setActiveContent(content, True);
    }
}

void QtMainWindow::onViewNextContentAction()
{
    common::UiController &uiCtrl = common::Application::instance()->ui();
    common::Content *content = uiCtrl.nextContent();

    if (content) {
        uiCtrl.setActiveContent(content, True);
    }
}

void QtMainWindow::onOpenRecentProject(bool)
{
    QString location = sender()->property("location").toString();
    openProject(location);
}

void QtMainWindow::onClearAllRecentProjects(bool)
{
    settings().set("o3s::main::project::recents", QVariant(QStringList()));
    initRecentProjectsMenu();
}

void QtMainWindow::onOpenRecentResources(bool)
{
    // @todo
}

void QtMainWindow::onClearAllRecentResources(bool)
{
    settings().set("o3s::main::resource::recents", QVariant(QStringList()));
    initRecentResourcesMenu();
}

void QtMainWindow::onChangeMainTitle(const QString &title)
{
    if (title.isEmpty()) {
        setWindowTitle(tr("Objective-3D Studio"));
    } else {
        setWindowTitle(title + " - " + tr("Objective-3D Studio"));
    }
}

void QtMainWindow::closeWorkspace()
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        if (workspace->hasChanges()) {
            workspace->save();
        }

        common::Application::instance()->workspaces().closeCurrent();
    }
}

o3d::studio::common::Settings &QtMainWindow::settings()
{
    return common::Application::instance()->settings();
}

void QtMainWindow::initRecentProjectsMenu()
{
    common::Settings &settings = common::Application::instance()->settings();
    QStringList recentsProject = settings.get("o3s::main::project::recents", QVariant(QStringList())).toStringList();

    QAction *action;
    foreach (action, ui.menuRecentsProjects->actions()) {
        if (action->property("location").isValid()) {
            ui.menuRecentsProjects->removeAction(action);
        }
    }

    QAction *separator = ui.menuRecentsProjects->actions().at(0);

    QString project;
    int i = 0;
    foreach (project, recentsProject) {
        if (project.isEmpty()) {
            continue;
        }

        QAction *action = new QAction(QString("&%1 | %2").arg(i).arg(project));
        action->setProperty("location", project);
        connect(action, SIGNAL(triggered(bool)), SLOT(onOpenRecentProject(bool)));
        ui.menuRecentsProjects->insertAction(separator, action);

        ++i;
    }
}

void QtMainWindow::initRecentResourcesMenu()
{
    common::Settings &settings = common::Application::instance()->settings();
    QStringList recentsResource = settings.get("o3s::main::resource::recents", QVariant(QStringList())).toStringList();

    QAction *action;
    foreach (action, ui.menuRecentsResources->actions()) {
        if (action->property("location").isValid()) {
            ui.menuRecentsResources->removeAction(action);
        }
    }

    QAction *separator = ui.menuRecentsResources->actions().at(0);

    int i = 0;
    QString resource;
    foreach (resource, recentsResource) {
        if (resource.isEmpty()) {
            continue;
        }

        QAction *action = new QAction(QString("&%1 | %2").arg(i).arg(resource));
        action->setProperty("location", resource);
        connect(action, SIGNAL(triggered(bool)), SLOT(onOpenRecentResources(bool)));
        ui.menuRecentsResources->insertAction(separator, action);

        ++i;
    }
}

void QtMainWindow::openProject(const QString &location)
{
    // remove from list
    QStringList recentsProject = settings().get("o3s::main::project::recents", QVariant(QStringList())).toStringList();
    recentsProject.removeOne(location);
    settings().set("o3s::main::project::recents", QVariant(recentsProject));

    initRecentProjectsMenu();

    QStringList parts = location.split(QDir::separator());
    if (parts.length() == 0) {
        return;
    }

    QString name = parts[parts.length()-1];
    QString path = toQString(common::Application::instance()->workspaces().defaultProjectsPath().getFullPathName());

    if (parts.length() > 1) {
        parts.removeLast();
        path = parts.join(QDir::separator());
        if (path.endsWith("/")) {
            path.chop(1);
        }
    }

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace->hasProject(fromQString(location))) {
        // currently loaded
        return;
    }

    common::Project *project = new common::Project(fromQString(name), workspace);
    project->setLocation(fromQString(path));

    try {
        project->load();
    } catch (common::E_ProjectException &e) {
        delete project;
        messenger().warning(e.getMsg());
        QMessageBox::warning(this, tr("Project warning"), toQString(e.getMsg()));
        return;
    }

    workspace->addProject(project);
    project->setupMasterScene();

    messenger().info(fromQString(tr("Project %1 successfuly open").arg(name)));

    workspace->setActiveProject(project->ref().light());
}

void QtMainWindow::openResource(const QString &location)
{
    Q_UNUSED(location)
    // @todo
}

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/importer/importer.h"
#include "o3d/studio/common/importer/importdefinition.h"

void QtMainWindow::importFile(const QString &location)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (!workspace) {
        return;
    }

    common::Project *project = workspace->activeProject();
    if (!project) {
        return;
    }

    // @todo might use the ImportFileDialog with generals options,
    // plus a sub-panel dynamic related to the importer module itself.

    // from the selection manager take the first selected hub or project in way to
    // import into this node. if project is selected import into the root hub, same if
    // no selection at all.

    // find the importer
    o3d::File fileInfo(fromQString(location));
    String fileExt = String("*.") + fileInfo.getFileExt();

    const common::Importer *importer = nullptr;

    for (const common::Importer *_importer : common::Application::instance()->instance()->importers().importerList()) {
        if (_importer->exts().sub(fileExt, 0) != -1) {
            importer = _importer;
            break;
        }
    }

    const std::set<common::SelectionItem *> currentSelection =
            common::Application::instance()->selection().filterCurrentByBaseType(
                common::TypeRef::hub());

    o3d::studio::common::Hub *parentHub = project->rootHub();

    if (currentSelection.size() > 0) {
        parentHub = static_cast<common::Hub*>(project->lookup((*currentSelection.begin())->ref()));
//        if (!parentHub.acceptAnyHub()) { @todo
//            parentHub = project->rootHub();
//        }
    }

    if (parentHub) {
        common::ImporterOption *options = new common::ImporterOption();

        common::Importer *importer = common::Application::instance()->importers().importer("o3s::plugin::importer::fbxi");
        common::ImportDefinition *def = nullptr;

        try {
            def = importer->import(
                                fileInfo.getFullFileName(),
                                options,
                                parentHub);
        } catch (E_BaseException &e) {
            common::Application::instance()->messenger().critical(e.getMsg());
        }

        deletePtr(def);
    }
}

o3d::studio::common::Messenger &QtMainWindow::messenger()
{
    return common::Application::instance()->messenger();
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

void QtMainWindow::loadLanguage(const String &language)
{
    common::Application::instance()->loadLanguage(language);

    if (m_currentLanguage != language) {
        QLocale locale = QLocale::system();
        QString languageCode = toQString(language);

        if (language != "default") {
            locale = QLocale(toQString(language));
        } else {
            languageCode = locale.name().split("_").at(0);
        }

        QString languageName = QLocale::languageToString(locale.language());

        switchTranslator(m_translator, QString("o3smain_%1.qm").arg(languageCode));
        m_currentLanguage = language;
    }
}

//
// AppWindow standalone
//

MainWindow::MainWindow(BaseObject *) :
    AppWindow()
{
    m_qtMainWindow = new QtMainWindow();

    setTitle("Objective-3D Studio");
    // setIcon("rc/main/icons/32x32/logo.png");

    // connections to settings controller
    common::Application::instance()->settings().settingChanged.connect(this, &MainWindow::onSettingChanged);

    // connections to command manager (async)
    common::Application::instance()->command().commandDone.connect(this, &MainWindow::onCommandDone, CONNECTION_ASYNCH);
    common::Application::instance()->command().commandUpdate.connect(this, &MainWindow::onCommandUpdate, CONNECTION_ASYNCH);

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &MainWindow::onSelectionChanged);

    // message
    common::Messenger& messenger = common::Application::instance()->messenger();
    messenger.onNewMessage.connect(this, &MainWindow::onMessage, CONNECTION_ASYNCH);

    o3d::studio::common::UiController &ui = common::Application::instance()->ui();

    // connections to ui controller
    ui.attachContent.connect(this, &MainWindow::onAttachContent);
    ui.attachDock.connect(this, &MainWindow::onAttachDock);
    ui.attachToolBar.connect(this, &MainWindow::onAttachToolBar);

    ui.detachContent.connect(this, &MainWindow::onDetachContent);
    ui.detachDock.connect(this, &MainWindow::onDetachDock);
    ui.detachToolBar.connect(this, &MainWindow::onDetachToolBar);

    ui.showContent.connect(this, &MainWindow::onShowContent);

    // workspace manager
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &MainWindow::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());  

    // setup the main ui
    applySettings();
    m_qtMainWindow->setup();
}

MainWindow::~MainWindow()
{
    disconnect();

    commitSettings();

    common::UiController &uiCtrl = common::Application::instance()->ui();
    uiCtrl.disconnect(this);

    // docks
    for (auto it = m_docks.begin(); it != m_docks.end(); ++it) {
        common::Dock *dock = it->second;

        if (!uiCtrl.removeDock(it->first)) {
            dock->ui()->setParent(nullptr);
        }
        delete dock;
    }

    // toolbars
    for (auto it = m_toolBars.begin(); it != m_toolBars.end(); ++it) {
        common::ToolBar *toolBar = it->second;

        if (!uiCtrl.removeToolBar(it->first)) {
            toolBar->ui()->setParent(nullptr);
        }
        delete toolBar;
    }

    // contents
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it) {
        common::Content *content = it->second;

        if (!uiCtrl.removeContent(it->first)) {
            content->ui()->setParent(nullptr);
        }
        delete content;
    }

    delete m_qtMainWindow;
}

QMainWindow *MainWindow::mainWindow()
{
    return m_qtMainWindow;
}

const QMainWindow *MainWindow::mainWindow() const
{
    return m_qtMainWindow;
}

o3d::Bool MainWindow::applySettings()
{
    common::Settings &settings = common::Application::instance()->settings();

    m_qtMainWindow->resize(settings.get("o3s::main::window::size", QVariant(QSize(800, 400))).toSize());
    m_qtMainWindow->move(settings.get("o3s::main::window::position", QVariant(QPoint(100, 100))).toPoint());

    m_qtMainWindow->loadLanguage(fromQString(settings.get("o3s::main::language", QVariant("default")).toString()));
    m_qtMainWindow->setThemeColor(fromQString(settings.get("o3s::main::theme::color", QVariant("dark")).toString()));

    return True;
}

o3d::Bool MainWindow::commitSettings()
{
    common::Settings &settings = common::Application::instance()->settings();

    settings.set("o3s::main::window::size", QVariant(m_qtMainWindow->size()));
    settings.set("o3s::main::window::position", QVariant(m_qtMainWindow->pos()));

    return True;
}

void MainWindow::onMessage(o3d::UInt32 msgType, String message)
{
    if (msgType == common::Messenger::INFO_MSG || msgType == common::Messenger::STATUS_MSG) {
        m_qtMainWindow->statusBar()->showMessage(toQString(message));
    }
}

void MainWindow::onAttachContent(o3d::String name, o3d::studio::common::Content *content)
{
    addContentWidget(name, content);
}

void MainWindow::onAttachDock(o3d::String name, o3d::studio::common::Dock *dock)
{
    setupDock(name, dock, dock->dockWidgetArea());
}

void MainWindow::onAttachToolBar(o3d::String name, o3d::studio::common::ToolBar *toolBar)
{
    setupToolBar(name, toolBar, toolBar->toolBarArea());
}

void MainWindow::onDetachContent(o3d::String name, o3d::studio::common::Content *)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        removeContentWidget(name);
    }
}

void MainWindow::onDetachDock(o3d::String name, o3d::studio::common::Dock *dock)
{
    auto it = m_docks.find(name);
    if (it != m_docks.end()) {
        m_docks.erase(it);
        m_qtMainWindow->removeDockWidget(dock->ui());
    }
}

void MainWindow::onDetachToolBar(o3d::String name, o3d::studio::common::ToolBar *toolBar)
{
    auto it = m_toolBars.find(name);
    if (it != m_toolBars.end()) {
        m_toolBars.erase(it);
        m_qtMainWindow->removeToolBar(toolBar->ui());
    }
}

void MainWindow::onShowContent(String name, o3d::studio::common::Content *content, Bool showHide)
{
   if (content) {
        if (showHide) {
            setCurrentContentWidget(name);
        } else {

        }
    }
}

o3d::Bool MainWindow::setupDock(const String &name, o3d::studio::common::Dock *dock, Qt::DockWidgetArea area)
{
    if (m_docks.find(name) != m_docks.end()) {
        return False;
    }

    // Qt::Orientation orientation ?
    m_qtMainWindow->addDockWidget(area, dock->ui());
    dock->ui()->setMinimumWidth(200);
    dock->ui()->setMinimumHeight(200);
    dock->ui()->setProperty("name", QVariant(toQString(name)));
    m_docks[dock->elementName()] =  dock;

    // add view menu action
    int i = m_qtMainWindow->ui.menuDockViews->actions().size();
    QString title = "";
    if (i < 10) {
        title = QString("&%1 | %2").arg(i).arg(dock->ui()->windowTitle());
    } else {
        title = QString("%1").arg(i).arg(dock->ui()->windowTitle());
    }

    QAction *action = new QAction(common::UiUtils::tintIcon(dock->ui()->windowIcon()), title);
    action->setProperty("name", toQString(name));

    if (i < 10) {
        action->setShortcut(QKeySequence(QString("Alt+Shift+%1").arg(i)));
    }

    m_qtMainWindow->connect(action, SIGNAL(triggered(bool)), SLOT(onViewDock()));

    m_qtMainWindow->ui.menuDockViews->addAction(action);

    return True;
}

o3d::Bool MainWindow::removeDock(const String &name)
{
    // erase menu entry
    QList<QAction*> actions = m_qtMainWindow->ui.menuDockViews->actions();
    QAction *action = nullptr;
    foreach (action, actions) {
        if (action->property("name").toString() == toQString(name)) {
            m_qtMainWindow->ui.menuDockViews->removeAction(action);
            return True;
        }
    }

    return False;
}

o3d::Bool MainWindow::setupToolBar(const String &name, o3d::studio::common::ToolBar *toolBar, Qt::ToolBarArea area)
{
    if (m_toolBars.find(name) != m_toolBars.end()) {
        return False;
    }

    m_qtMainWindow->addToolBar(area, toolBar->ui());
    toolBar->ui()->setMinimumWidth(48);
    toolBar->ui()->setMinimumHeight(48);
    toolBar->ui()->setProperty("name", QVariant(toQString(name)));
    m_toolBars[toolBar->elementName()] = toolBar;

    // add view menu action
    int i = m_qtMainWindow->ui.menuToolBarViews->actions().size();
    QString title = "";
    if (i < 10) {
        title = QString("&%1 | %2").arg(i).arg(toolBar->ui()->windowTitle());
    } else {
        title = QString("%1").arg(i).arg(toolBar->ui()->windowTitle());
    }

    QAction *action = new QAction(common::UiUtils::tintIcon(toolBar->ui()->windowIcon()), title);
    action->setProperty("name", toQString(name));

    if (i < 10) {
        action->setShortcut(QKeySequence(QString("Ctrl+Alt+Shift+%1").arg(i)));
    }

    m_qtMainWindow->connect(action, SIGNAL(triggered(bool)), SLOT(onViewToolBar()));

    m_qtMainWindow->ui.menuToolBarViews->addAction(action);

    return True;
}

o3d::Bool MainWindow::removeToolBarWidget(const String &name)
{
    // erase menu entry
    QList<QAction*> actions = m_qtMainWindow->ui.menuToolBarViews->actions();
    QAction *action = nullptr;
    foreach (action, actions) {
        if (action->property("name").toString() == toQString(name)) {
            m_qtMainWindow->ui.menuToolBarViews->removeAction(action);
            return True;
        }
    }

    return False;
}

o3d::Bool MainWindow::addContentWidget(const String &name, o3d::studio::common::Content *content)
{
    if (m_contents.find(name) != m_contents.end()) {
        return False;
    }

    m_contents[content->elementName()] = content;

    // add view menu action
    int i = m_qtMainWindow->ui.menuContentViews->actions().size();
    QString title = "";
    if (i < 10) {
        title = QString("&%1 | %2").arg(i).arg(content->ui()->windowTitle());
    } else {
        title = QString("%1").arg(i).arg(content->ui()->windowTitle());
    }

    QAction *action = new QAction(common::UiUtils::tintIcon(content->ui()->windowIcon()), title);
    action->setProperty("name", toQString(name));

    if (i < 10) {
        action->setShortcut(QKeySequence(QString("Alt+%1").arg(i)));
    }

    m_qtMainWindow->connect(action, SIGNAL(triggered(bool)), SLOT(onViewContent()));

    m_qtMainWindow->ui.menuContentViews->addAction(action);
    m_qtMainWindow->m_stackedWidget->addWidget(content->ui());

    return True;
}

o3d::Bool MainWindow::removeContentWidget(const String &name)
{
    // cannot remove the welcome content widget
    if (name == "welcome") {
        return False;
    }

    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        if (m_currentContent && m_currentContent == it->second) {
            m_currentContent->ui()->setParent(nullptr);
            m_currentContent = nullptr;

            setCurrentContentWidget("o3s::main::browsercontent");
        }

        it->second->ui()->setParent(nullptr);
        m_contents.erase(it);

        // erase menu entry, but we have to reprocess it because of orders
        for (QAction *action : m_qtMainWindow->ui.menuContentViews->actions()) {
            m_qtMainWindow->ui.menuContentViews->removeAction(action);
        }

        int i = 0;
        o3d::studio::common::Content *content = nullptr;
        QString title = "";
        QAction *action = nullptr;
        for (auto it2 = m_contents.begin(); it2 != m_contents.end(); ++it2) {
            content = it2->second;

            if (i < 10) {
                title = QString("&%1 | %2").arg(i).arg(content->ui()->windowTitle());
            } else {
                title = QString("%1").arg(i).arg(content->ui()->windowTitle());
            }

            action = new QAction(common::UiUtils::tintIcon(content->ui()->windowIcon()), title);
            action->setProperty("name", toQString(content->elementName()));

            if (i < 10) {
                action->setShortcut(QKeySequence(QString("Alt+%1").arg(i)));
            }

            m_qtMainWindow->connect(action, SIGNAL(triggered(bool)), SLOT(onViewContent()));
            m_qtMainWindow->ui.menuContentViews->addAction(action);
        }

        return True;
    }

    return False;
}

o3d::Bool MainWindow::setCurrentContentWidget(const String &name)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        // already current
        if (m_currentContent && m_currentContent == it->second) {
            return True;
        }

        // setup
        m_qtMainWindow->m_stackedWidget->setCurrentWidget(it->second->ui());
        m_currentContent = it->second;

        // adapt window title
        if (m_currentContent->ui()->windowTitle().isEmpty()) {
            m_qtMainWindow->setWindowTitle(m_qtMainWindow->tr("Objective-3D Studio"));
        } else {
            m_qtMainWindow->setWindowTitle(m_currentContent->ui()->windowTitle() + " - " + m_qtMainWindow->tr("Objective-3D Studio"));
        }

        return True;
    }

    return False;
}

o3d::studio::common::Content *MainWindow::contentWidget(const String &name)
{
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

const o3d::studio::common::Content *MainWindow::contentWidget(const String &name) const
{
    auto cit = m_contents.find(name);
    if (cit != m_contents.cend()) {
        return cit->second;
    } else {
        return nullptr;
    }
}

void MainWindow::onChangeCurrentWorkspace(const String&)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectAdded.connect(this, &MainWindow::onProjectAdded);
        workspace->onProjectActivated.connect(this, &MainWindow::onProjectActivated);
        workspace->onProjectRemoved.connect(this, &MainWindow::onProjectRemoved);

        // no more opened projects
        m_qtMainWindow->ui.actionCloseProject->setEnabled(false);
        m_qtMainWindow->ui.actionImport->setEnabled(false);
    }
}

void MainWindow::onSettingChanged(const String &key, const QVariant &value)
{
    if (key == "o3s::main::language") {
        QString language = value.toString();
        m_qtMainWindow->loadLanguage(fromQString(language));

        QLocale locale = QLocale::system();

        if (language != "default") {
            locale = QLocale(language);
        }

        QString languageName = QLocale::languageToString(locale.language());
        messenger().info(fromQString(tr("Current language changed to %1").arg(languageName)));
    } else if (key == "o3s::main::theme::color") {
        m_qtMainWindow->setThemeColor(fromQString(value.toString()));
        messenger().info(fromQString(tr("Current theme changed to %1").arg(toQString(m_currentTheme))));
    }
}

void MainWindow::onCommandUpdate()
{
    if (common::Application::instance()->command().hasDoneCommands()) {
        m_qtMainWindow->ui.actionUndo->setEnabled(true);

        String nextCmd = common::Application::instance()->command().nextToUndo();
        m_qtMainWindow->ui.actionUndo->setText(tr("Undo %1").arg(toQString(nextCmd)));
    } else {
        m_qtMainWindow->ui.actionUndo->setEnabled(false);
        m_qtMainWindow->ui.actionUndo->setText(tr("Undo"));
    }

    if (common::Application::instance()->command().hasUndoneCommands()) {
        m_qtMainWindow->ui.actionRedo->setEnabled(true);

        String nextCmd = common::Application::instance()->command().nextToRedo();
        m_qtMainWindow->ui.actionRedo->setText(tr("Redo %1").arg(toQString(nextCmd)));
    } else {
        m_qtMainWindow->ui.actionRedo->setEnabled(false);
        m_qtMainWindow->ui.actionRedo->setText(tr("Redo"));
    }
}

void MainWindow::onCommandDone(String /*name*/, String label, bool done)
{
    if (done) {
        // @todo move on CommandManager but need sync thread
        messenger().info(fromQString(tr("%1 done").arg(toQString(label))));
    } else {
        // @todo move on CommandManager but need sync thread
        messenger().info(fromQString(tr("%1 undone").arg(toQString(label))));
    }
}

void MainWindow::onSelectionChanged()
{
//    common::Selection &selection = common::Application::instance()->selection();
}

void MainWindow::onProjectAdded(common::LightRef ref)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::Project *project = workspace->project(ref);

    QStringList recentsProject = settings().get("o3s::main::project::recents", QVariant(QStringList())).toStringList();
    recentsProject.removeAll(toQString(project->path().getFullPathName()));
    recentsProject.push_front(toQString(project->path().getFullPathName()));

    while (recentsProject.size() > MAX_RECENTS_FILES) {
        recentsProject.pop_back();
    }

    settings().set("o3s::main::project::recents", QVariant(recentsProject));

    m_qtMainWindow->initRecentProjectsMenu();
}

void MainWindow::onProjectActivated(o3d::studio::common::LightRef /*ref*/)
{
    m_qtMainWindow->ui.actionCloseProject->setEnabled(true);
    m_qtMainWindow->ui.actionImport->setEnabled(true);
}

void MainWindow::onProjectRemoved(o3d::studio::common::LightRef /*ref*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    common::Project *project = workspace->activeProject();
    if (!project) {
        m_qtMainWindow->ui.actionCloseProject->setEnabled(false);
        m_qtMainWindow->ui.actionImport->setEnabled(false);
    }
}

o3d::studio::common::Settings &MainWindow::settings()
{
    return common::Application::instance()->settings();
}

o3d::studio::common::Messenger &MainWindow::messenger()
{
    return common::Application::instance()->messenger();
}

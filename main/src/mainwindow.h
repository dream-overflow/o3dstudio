/**
 * @brief Application main window
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-17
 * @details
 */

#ifndef _O3DS_MAIN_MAINWINDOW_H
#define _O3DS_MAIN_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QTranslator>

#include "o3d/studio/common/capacitysettings.h"
#include "o3d/studio/common/settings.h"
#include "o3d/studio/common/messenger.h"
#include "o3d/studio/common/objectref.h"

#include "o3d/studio/common/ui/content.h"
#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/ui/toolbar.h"

#include <o3d/core/appwindow.h>

#include "ui_mainwindow.h"

namespace o3d {
namespace studio {
namespace main {

class QtMainWindow;

using o3d::studio::common::LightRef;

/**
 * @brief The MainWindow class
 * For the momoent it is the O3D adaptor from Qt main window. But could replace the QtMainWindow later.
 */
class MainWindow : public AppWindow, public common::CapacitySettings
{
    Q_DECLARE_TR_FUNCTIONS(MainWindow)

public:

    MainWindow(BaseObject *parent = nullptr);
    virtual ~MainWindow();

    QMainWindow *mainWindow();
    const QMainWindow *mainWindow() const;

    //
    // CapacitySettings
    //

    virtual Bool applySettings() override;
    virtual Bool commitSettings() override;

    //
    // Widgets
    //

    Bool setupDock(const String &name, common::Dock *dock, Qt::DockWidgetArea area);
    Bool removeDock(const String &name);

    Bool setupToolBar(const String &name, common::ToolBar *toolBar, Qt::ToolBarArea area);
    Bool removeToolBarWidget(const String &name);

    Bool addContentWidget(const String &name, common::Content *widget);
    Bool removeContentWidget(const String &name);
    Bool setCurrentContentWidget(const String &name);
    common::Content* contentWidget(const String &name);
    const common::Content *contentWidget(const String &name) const;

private /*slots*/:

    void onMessage(UInt32 msgType, const String &message);

    void onAttachContent(String, common::Content*);
    void onAttachDock(String, common::Dock*);
    void onAttachToolBar(String, common::ToolBar*);

    void onDetachContent(String, common::Content*);
    void onDetachDock(String, common::Dock*);
    void onDetachToolBar(String, common::ToolBar*);

    void onShowContent(String name, common::Content *content, Bool showHide);

    void onSettingChanged(const String &key, const QVariant &value);

    void onCommandUpdate();
    void onCommandDone(String name, String label, Bool done);

    void onChangeCurrentWorkspace(const String &name);
    void onProjectAdded(common::LightRef ref);

    void onSelectionChanged();

private:

    QtMainWindow *m_qtMainWindow;

    String m_currentTheme;                //!< current active theme name
    String m_currentLanguage;             //!< current active language 2 letters code

    common::Content *m_currentContent{nullptr};      //!< current active content widget

    std::map<o3d::String, common::Content*> m_contents;   //!< existings content widget (only once activated)
    std::map<o3d::String, common::ToolBar*> m_toolBars;   //!< existings toolbars (many can be activated)
    std::map<o3d::String, common::Dock*> m_docks;         //!< existings docks (many can be activated)

    common::Settings& settings();
    common::Messenger& messenger();
};

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MainWindow;

public:

    QtMainWindow(QWidget *parent = nullptr);
    virtual ~QtMainWindow();

    Bool setThemeColor(const String &theme);

    const String &language() const;
    const String& theme() const;

protected:

    virtual void closeEvent(QCloseEvent*);

    // this event is called, when a new translator is loaded or the system language is changed
    virtual void changeEvent(QEvent*);

    // process o3d evt manager
    virtual void customEvent(QEvent*);

private slots:

    void onFileNewProject();
    void onFileOpenProject();
    void onFileMenuSave();
    void onFileMenuSaveAll();
    void onFileMenuSaveAs();

    void onFileNewResource();

    void onFileWorkspaceManage();

    void onFileMenuPreferences();
    void onFileMenuClose();
    void onFileMenuQuit();

    void onWindowFullScreen();

    void onViewHomePage();

    void onViewContent();
    void onViewDock();
    void onViewToolBar();

    void onHelpIndex();
    void onSystemInfo();
    void onAboutPlugin();
    void onAbout();

    void onOpenRecentProject(bool);
    void onClearAllRecentProjects(bool);

    void onOpenRecentResources(bool);
    void onClearAllRecentResources(bool);

    void onViewPreviousContentAction();
    void onViewNextContentAction();

    void onChangeMainTitle(const QString &title);

    void onUndoAction();
    void onRedoAction();

private:

    Ui::MainWindow ui;

    QTranslator m_translator;     //!< contains the translations for o3smain

    void closeWorkspace();

    QPalette m_originalPalette;
    String m_currentTheme;                 //!< current active theme name
    String m_currentLanguage;              //!< current active language 2 letters code

    QStatusBar *m_statusBar{nullptr};      //!< unique bottom status bar

    common::Settings& settings();

    void initRecentProjectsMenu();
    void initRecentResourcesMenu();

    void openProject(const QString &location);
    void openResource(const QString &location);

    common::Messenger& messenger();

    void setup();

    // loads a language by the given language shortcurt (e.g. de, en)
    void loadLanguage(const String &language);
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINWINDOW_H

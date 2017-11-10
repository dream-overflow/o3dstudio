/**
 * @brief Application main window
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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

#include "ui_mainwindow.h"

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

class MainWindow : public QMainWindow, public common::CapacitySettings
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    QDockWidget* createDock(const QString &title, const QString &name, Qt::DockWidgetArea area);
    bool setupDock(const QString &name, QDockWidget *dock, Qt::DockWidgetArea area);
    bool removeDock(const QString &name);

    QToolBar* createToolBar(const QString &title, const QString &name, Qt::ToolBarArea area);
    bool setupToolBar(const QString &name, QToolBar *toolBar, Qt::ToolBarArea area);
    bool removeToolBarWidget(const QString &name);

    bool addContentWidget(const QString &name, QWidget *widget);
    bool removeContentWidget(const QString &name);
    bool setCurrentContentWidget(const QString &name);
    QWidget* contentWidget(const QString &name);
    const QWidget* contentWidget(const QString &name) const;

    bool setThemeColor(const QString &theme);

    const QString& language() const;
    const QString& theme() const;

    //
    // CapacitySettings
    //

    virtual bool applySettings() override;
    virtual bool commitSettings() override;

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

    void onSettingChanged(const QString &key, const QVariant &value);

    void onAttachContent(QString, QWidget*);
    void onAttachDock(QString, QDockWidget*, Qt::DockWidgetArea area);
    void onAttachToolBar(QString, QToolBar*, Qt::ToolBarArea area);

    void onDetachContent(QString, QWidget*);
    void onDetachDock(QString, QDockWidget*);
    void onDetachToolBar(QString, QToolBar*);

    void onUndoAction();
    void onRedoAction();

    void onViewPreviousContentAction();
    void onViewNextContentAction();

    void onShowContent(QString name, QWidget *content, bool showHide);

    void onCommandDone(QString name, QString label, bool done);

    void onOpenRecentProject(bool);
    void onClearAllRecentProjects(bool);

    void onOpenRecentResources(bool);
    void onClearAllRecentResources(bool);

    void onChangeCurrentWorkspace(const QString &name);
    void onProjectAdded(const LightRef &ref);

    void onChangeMainTitle(const QString &title);
    void onMessage(QtMsgType msgType, const QString &message);

private:

    Ui::MainWindow ui;

    bool m_darkTheme{false};

    QTranslator m_translator;     //!< contains the translations for o3smain

    void closeWorkspace();

    // loads a language by the given language shortcurt (e.g. de, en)
    void loadLanguage(const QString& language);

    QString m_currentTheme;                //!< current active theme name
    QString m_currentLanguage;             //!< current active language 2 letters code

    QStatusBar *m_statusBar{nullptr};      //!< unique bottom status bar
    QWidget *m_currentContent{nullptr};    //!< current active content widget

    QMap<QString, QWidget*> m_contents;    //!< existings content widget (only once activated)

    QMap<QString, QToolBar*> m_toolBars;   //!< existings toolbars (many can be activated)
    QMap<QString, QDockWidget*> m_docks;   //!< existings docks (many can be activated)

    common::Settings& settings();

    void initRecentProjectsMenu();
    void initRecentResourcesMenu();

    void openProject(const QString &location);
    void openResource(const QString &location);

    common::Messenger& messenger();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINWINDOW_H

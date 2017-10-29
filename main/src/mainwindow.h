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

#include "common/capacity/capacitysettings.h"

#include "ui_mainwindow.h"

namespace o3d {
namespace studio {
namespace main {

class MainWindow : public QMainWindow, public common::CapacitySettings
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    QDockWidget* createDock(const QString &title, const QString &name, Qt::DockWidgetArea area);
    bool setupDock(const QString &name, QDockWidget *dock, Qt::DockWidgetArea area);

    QToolBar* createToolBar(const QString &title, const QString &name, Qt::ToolBarArea area);
    bool setupToolBar(const QString &name, QToolBar *toolBar, Qt::ToolBarArea area);

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

signals:

    void setCurrentLanguage(const QString &language);

protected:

    virtual void closeEvent(QCloseEvent*);

    // this event is called, when a new translator is loaded or the system language is changed
    virtual void changeEvent(QEvent*);

private slots:

    void onFileNewProject();
    void onFileNewResource();
    void onFileMenuPreferences();
    void onFileMenuClose();
    void onFileMenuQuit();
    void onWindowFullScreen();
    void onFileWorkspaceManage();
    void onViewHomePage();
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

    QMap<QString, QWidget*> m_contents;   //!< existings content widget (only once activated)

    QMap<QString, QToolBar*> m_toolBars;   //!< existings toolbars (many can be activated)
    QMap<QString, QDockWidget*> m_docks;   //!< existings docks (many can be activated)
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINWINDOW_H

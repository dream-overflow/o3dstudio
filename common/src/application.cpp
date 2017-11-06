/**
 * @brief Common application singleton
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include<QtCore/QDir>
#include<QtCore/QUrl>
#include<QtCore/QThread>
#include<QtCore/QCoreApplication>
#include<QtCore/QLibraryInfo>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/modulemanager.h"

#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/storage/store.h"

using namespace o3d::studio::common;

Application* Application::m_instance = nullptr;

Application::Application()
{
    // avoid recursive call when ctor call himself
    m_instance = (Application*)this;

    // get working directory as path
    m_workingDir = QDir::currentPath();
    m_appDir = QCoreApplication::applicationDirPath();

    m_workspaceManager = new WorkspaceManager();
    m_ui = new UiController();
    m_commandManager = new CommandManager();
    m_store = new Store();
}

Application::~Application()
{
    delete m_workspaceManager;
    delete m_ui;
    delete m_commandManager;
    delete m_store;
}

// Singleton instantiation
Application* Application::instance()
{
    if (!m_instance) {
        m_instance = new Application();
    }

    return m_instance;
}

// Singleton destruction
void Application::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

const QString &Application::appDir() const
{
    return m_appDir;
}

const QString& Application::workingDir() const
{
    return m_workingDir;
}

Settings &Application::settings()
{
    return m_settings;
}

const Settings &Application::settings() const
{
    return m_settings;
}

WorkspaceManager &Application::workspaceManager()
{
    return *m_workspaceManager;
}

const WorkspaceManager &Application::workspaceManager() const
{
    return *m_workspaceManager;
}

UiController &Application::ui()
{
    return *m_ui;
}

const UiController &Application::ui() const
{
    return *m_ui;
}

CommandManager &Application::command()
{
    return *m_commandManager;
}

const CommandManager &Application::command() const
{
    return *m_commandManager;
}

Store &Application::store()
{
    return *m_store;
}

const Store &Application::store() const
{
    return *m_store;
}

bool Application::start()
{
    m_settings.loadAll();

    ModuleManager::instance()->setPluginsPath(m_settings.get("o3s::plugin::path").toUrl().toLocalFile());
    ModuleManager::instance()->setPluginsFilters(m_settings.get("o3s::plugin::exts").toStringList());
    ModuleManager::instance()->searchModules();

    m_commandManager->begin();


    // alway load o3sdummy for testing
    Module *o3sdummy = ModuleManager::instance()->load("o3sdummy");
    if (o3sdummy != nullptr) {
        o3sdummy->start();
    }

    m_started = true;
    return true;
}

bool Application::stop()
{
    m_settings.saveAll();

    m_commandManager->finish();

    while (m_commandManager->hasPendingCommands() || m_commandManager->hasRunningCommands()) {
        m_commandManager->wait(20);
    }

    ModuleManager::instance()->unloadAll();
    ModuleManager::destroy();

    m_started = false;
    return true;
}

void switchTranslator(QTranslator& translator, const QString& filename, QString path = QString())
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if (path.isEmpty()) {
        path = QDir::currentPath() + QDir::separator() + QString(LANGUAGES_PATH);
    }

    if (translator.load(filename, path, QString(), QString())) {
        qApp->installTranslator(&translator);
    }
}

void Application::loadLanguage(const QString &language)
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

        switchTranslator(m_translator, QString("o3scommon_%1.qm").arg(languageCode));
        switchTranslator(
                    m_translatorQt,
                    QString("qt_%1.qm").arg(languageCode),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));

        m_currentLanguage = language;
    }
}

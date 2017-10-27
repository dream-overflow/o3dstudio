/**
 * @brief Common application singleton
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include<QtCore/QDir>
#include<QtCore/QUrl>
#include<QtCore/QCoreApplication>

#include "common/application.h"
#include "common/modulemanager.h"
#include "common/workspace/workspacemanager.h"

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
}

Application::~Application()
{
    delete m_workspaceManager;
}

// Singleton instantiation
Application* Application::instance()
{
    if (!m_instance)
        m_instance = new Application();
    return m_instance;
}

// Singleton destruction
void Application::destroy()
{
    if (m_instance)
    {
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

bool Application::start()
{
    m_settings.loadAll();

    ModuleManager::instance()->setPluginsPath(m_settings.get("o3s::plugin::path").toUrl().toLocalFile());
    ModuleManager::instance()->setPluginsFilters(m_settings.get("o3s::plugin::exts").toStringList());
    ModuleManager::instance()->searchModules();

    // alway load o3sdummy for testing
    ModuleManager::instance()->load("o3sdummy");

    m_started = true;
    return true;
}

bool Application::stop()
{
    m_settings.saveAll();

    ModuleManager::instance()->unloadAll();
    ModuleManager::destroy();

    m_started = false;
    return true;
}

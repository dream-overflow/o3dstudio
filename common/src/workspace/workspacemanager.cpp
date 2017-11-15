/**
 * @brief Common workspace manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#include "o3d/studio/common/application.h"

#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"

using namespace o3d::studio::common;

WorkspaceManager::WorkspaceManager()
{
    if (!QDir::home().exists(".o3dstudio")) {
        if (!QDir::home().mkdir(".o3dstudio")) {
            throw;
        }
    }

    m_defaultPath = QDir::home();
    if (!m_defaultPath.cd(".o3dstudio")) {
        throw;
    }

    if (!m_defaultPath.exists("projects")) {
        m_defaultPath.mkdir("projects");
    }

    if (!m_defaultPath.isReadable()) {
        throw;
    }

    m_defaultProjectsPath = m_defaultPath.absoluteFilePath("projects");

    if (!m_defaultProjectsPath.isReadable()) {
        throw;
    }

    if (!m_defaultPath.exists("workspaces")) {
        m_defaultPath.mkdir("workspaces");
    }

    m_defaultWorkspacesPath = m_defaultPath.absoluteFilePath("workspaces");

    if (!m_defaultWorkspacesPath.isReadable()) {
        throw;
    }

    // check/default settings
    Settings& settings = Application::instance()->settings();

    if (!settings.has("o3s::main::project::previous-folder") ||
        settings.get("o3s::main::project::previous-folder").toString().isEmpty() ||
        !QDir(settings.get("o3s::main::project::previous-folder").toString()).exists()) {

        settings.set(
            "o3s::main::project::previous-folder",
            QVariant(m_defaultProjectsPath.absolutePath()));
    }

    // setup a new empty workspace
    m_current = new Workspace("default");
    m_current->setUuid(QUuid::createUuid());
}

WorkspaceManager::~WorkspaceManager()
{
    if (m_current) {
        if (m_current->hasChanges()) {
            m_current->save();
        }

        delete m_current;
    }
}

QStringList WorkspaceManager::workspaces() const
{
    return m_foundWorkspaces;
}

Workspace *WorkspaceManager::addWorkspace(const QString &name)
{
    if (m_foundWorkspaces.contains(name)) {
        return nullptr;
    }

    if (m_current->name() == name) {
        return nullptr;
    }

    Workspace *workspace = new Workspace(name);
    m_foundWorkspaces.append(name);

    emit onWorkspaceAdded(name);

    return workspace;
}

Workspace *WorkspaceManager::workspace(const QString &name)
{
    Q_UNUSED(name)

    return nullptr;
}

const Workspace *WorkspaceManager::workspace(const QString &name) const
{
    Q_UNUSED(name)

    return nullptr;
}

bool WorkspaceManager::deleteWorkspace(const QString &name)
{
    if (!m_foundWorkspaces.contains(name)) {
        return false;
    }

    // @todo load it and delete it

    m_foundWorkspaces.removeOne(name);

    return true;
}

bool WorkspaceManager::hasWorkspace(const QString &name) const
{
    return m_foundWorkspaces.contains(name);
}

bool WorkspaceManager::loadWorkspace(const QString &name)
{
    if (m_current && m_current->name() == name) {
        return true;
    }

    if (!m_foundWorkspaces.contains(name)) {
        return false;
    }

    // @todo load it and set as current
    Workspace *workspace = nullptr;

    m_current = workspace;

    emit onWorkspaceActivated(name);

    return true;
}

Workspace *WorkspaceManager::current()
{
    return m_current;
}

bool WorkspaceManager::closeCurrent()
{
    if (m_current) {
        if (m_current->hasChanges()) {
            m_current->save();
        }

        // setup a new empty workspace
        m_current = new Workspace("default");
        m_current->setUuid(QUuid::createUuid());

        emit onWorkspaceActivated(m_current->name());

        return true;
    }

    return false;
}

const QDir &WorkspaceManager::defaultPath() const
{
    return m_defaultPath;
}

const QDir &WorkspaceManager::defaultProjectsPath() const
{
    return m_defaultProjectsPath;
}

const QDir &WorkspaceManager::defaultWorkspacesPath() const
{
    return m_defaultWorkspacesPath;
}

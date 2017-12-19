/**
 * @brief Common workspace manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#include "o3d/studio/common/application.h"

#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"

#include <algorithm>

#include <QtCore/QDir>
#include <QtCore/QUuid>

using namespace o3d::studio::common;

WorkspaceManager::WorkspaceManager() :
    BaseObject()
{
    String homePath = fromQString(QDir::home().absolutePath());
    LocalDir homeDir(homePath);

    if (homeDir.check(".o3dstudio") != BaseDir::SUCCESS) {
        if (homeDir.makeDir(".o3dstudio") != BaseDir::SUCCESS) {
            throw;
        }
    }

    m_defaultPath = homePath;
    if (!m_defaultPath.cd(".o3dstudio")) {
        throw;
    }

    if (m_defaultPath.check("projects") != BaseDir::SUCCESS) {
        m_defaultPath.makeDir("projects");
    }

    if (!m_defaultPath.isReadable()) {
        throw;
    }

    m_defaultProjectsPath = m_defaultPath.makeFullPathName("projects");

    if (!m_defaultProjectsPath.isReadable()) {
        throw;
    }

    if (m_defaultPath.check("workspaces") != BaseDir::SUCCESS) {
        m_defaultPath.makeDir("workspaces");
    }

    m_defaultWorkspacesPath = m_defaultPath.makeFullPathName("workspaces");

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
            toQString(m_defaultProjectsPath.getFullPathName()));
    }

    // setup a new empty workspace
    m_current = new Workspace("default");
    m_current->setUuid(QUuid::createUuid().toByteArray().data());
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

o3d::T_StringList WorkspaceManager::workspaces() const
{
    return m_foundWorkspaces;
}

Workspace *WorkspaceManager::addWorkspace(const String &name)
{
    if (std::find(m_foundWorkspaces.begin(), m_foundWorkspaces.end(), name) != m_foundWorkspaces.end()) {
        return nullptr;
    }

    if (m_current->name() == name) {
        return nullptr;
    }

    Workspace *workspace = new Workspace(name);
    m_foundWorkspaces.push_back(name);

    onWorkspaceAdded(name);

    return workspace;
}

Workspace *WorkspaceManager::workspace(const String &name)
{
    Q_UNUSED(name)

    return nullptr;
}

const Workspace *WorkspaceManager::workspace(const String &name) const
{
    Q_UNUSED(name)

    return nullptr;
}

o3d::Bool WorkspaceManager::deleteWorkspace(const String &name)
{
    auto it = std::find(m_foundWorkspaces.begin(), m_foundWorkspaces.end(), name);
    if (it == m_foundWorkspaces.end()) {
        return False;
    }

    // @todo load it and delete it

    m_foundWorkspaces.erase(it);

    return True;
}

o3d::Bool WorkspaceManager::hasWorkspace(const String &name) const
{
    return std::find(m_foundWorkspaces.cbegin(), m_foundWorkspaces.cend(), name) != m_foundWorkspaces.cend();
}

o3d::Bool WorkspaceManager::loadWorkspace(const String &name)
{
    if (m_current && m_current->name() == name) {
        return True;
    }

    auto it = std::find(m_foundWorkspaces.begin(), m_foundWorkspaces.end(), name);
    if (it == m_foundWorkspaces.end()) {
        return False;
    }

    // @todo load it and set as current
    Workspace *workspace = nullptr;

    m_current = workspace;

    onWorkspaceActivated(name);

    return True;
}

Workspace *WorkspaceManager::current()
{
    return m_current;
}

o3d::Bool WorkspaceManager::closeCurrent()
{
    if (m_current) {
        if (m_current->hasChanges()) {
            m_current->save();
        }

        deletePtr(m_current);

        // setup a new empty workspace
        m_current = new Workspace("default");
        m_current->setUuid(QUuid::createUuid().toByteArray().data());

        onWorkspaceActivated(m_current->name());

        return True;
    }

    return False;
}

const o3d::LocalDir &WorkspaceManager::defaultPath() const
{
    return m_defaultPath;
}

const o3d::LocalDir &WorkspaceManager::defaultProjectsPath() const
{
    return m_defaultProjectsPath;
}

const o3d::LocalDir &WorkspaceManager::defaultWorkspacesPath() const
{
    return m_defaultWorkspacesPath;
}

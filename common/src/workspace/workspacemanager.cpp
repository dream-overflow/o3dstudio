/**
 * @brief Common workspace manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#include "common/workspace/workspacemanager.h"
#include "common/workspace/workspace.h"

using namespace o3d::studio::common;

WorkspaceManager::WorkspaceManager()
{
    // @todo get last session loaded workspace
    m_current = new Workspace("default");
}

WorkspaceManager::~WorkspaceManager()
{
    if (m_current) {
        m_current->save();
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

    return workspace;
}

Workspace *WorkspaceManager::workspace(const QString &name)
{
    return nullptr;
}

const Workspace *WorkspaceManager::workspace(const QString &name) const
{
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

    return true;
}

Workspace *WorkspaceManager::current()
{
    return m_current;
}

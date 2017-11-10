/**
 * @brief Common workspace manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_WORKSPACEMANAGER_H
#define _O3DS_COMMON_WORKSPACEMANAGER_H

#include <QtCore/QStringList>
#include <QtCore/QDir>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class Workspace;

/**
 * @brief The WorkspaceManager final class
 */
class O3S_API WorkspaceManager : public QObject
{
    Q_OBJECT

public:

    WorkspaceManager();
    ~WorkspaceManager();

    /**
     * @brief Return the list of found workspaces.
     * @return
     */
    QStringList workspaces() const;

    Workspace* addWorkspace(const QString &name);

    Workspace* workspace(const QString &name);
    const Workspace* workspace(const QString &name) const;

    bool deleteWorkspace(const QString &name);
    bool hasWorkspace(const QString &name) const;

    /**
     * @brief Load and set as current workspace.
     * @param name
     */
    bool loadWorkspace(const QString &name);

    Workspace* current();
    const Workspace* current() const;

    /**
     * @brief Close the current workspace, and save changes, finally setup an empty new workspace
     * @return
     */
    bool closeCurrent();

    /**
     * @brief Default workspaces location directory.
     * @return
     */
    const QDir& defaultPath() const;

    const QDir& defaultProjectsPath() const;
    const QDir& defaultWorkspacesPath() const;

signals:

    void onWorkspaceAdded(const QString &name);
    void onWorkspaceActivated(const QString &name);

private:

    QDir m_defaultPath;
    QDir m_defaultProjectsPath;
    QDir m_defaultWorkspacesPath;

    QStringList m_foundWorkspaces;
    Workspace* m_current{nullptr};
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACEMANAGER_H

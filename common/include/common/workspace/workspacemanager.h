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

namespace o3d {
namespace studio {
namespace common {

class Workspace;

/**
 * @brief The WorkspaceManager final class
 */
class WorkspaceManager
{
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

private:

    QStringList m_foundWorkspaces;
    Workspace* m_current{nullptr};
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACEMANAGER_H

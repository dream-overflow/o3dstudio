/**
 * @brief Common workspace manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_WORKSPACEMANAGER_H
#define _O3DS_COMMON_WORKSPACEMANAGER_H

#include <o3d/core/diskdir.h>
#include <o3d/core/stringlist.h>
#include <o3d/core/baseobject.h>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class Workspace;

/**
 * @brief The WorkspaceManager final class
 */
class O3S_API WorkspaceManager : public BaseObject
{
public:

    WorkspaceManager();
    ~WorkspaceManager();

    /**
     * @brief Return the list of found workspaces.
     * @return
     */
    T_StringList workspaces() const;

    Workspace* addWorkspace(const String &name);

    Workspace* workspace(const String &name);
    const Workspace* workspace(const String &name) const;

    Bool deleteWorkspace(const String &name);
    Bool hasWorkspace(const String &name) const;

    /**
     * @brief Load and set as current workspace.
     * @param name
     */
    Bool loadWorkspace(const String &name);

    Workspace* current();
    const Workspace* current() const;

    /**
     * @brief Close the current workspace, and save changes, finally setup an empty new workspace
     * @return
     */
    Bool closeCurrent();

    /**
     * @brief Default workspaces location directory.
     * @return
     */
    const DiskDir& defaultPath() const;

    const DiskDir& defaultProjectsPath() const;
    const DiskDir& defaultWorkspacesPath() const;

public /*signals*/:

    Signal<const String &/*name*/> onWorkspaceAdded{this};
    Signal<const String &/*name*/> onWorkspaceActivated{this};

private:

    DiskDir m_defaultPath;
    DiskDir m_defaultProjectsPath;
    DiskDir m_defaultWorkspacesPath;

    T_StringList m_foundWorkspaces;
    Workspace* m_current{nullptr};
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACEMANAGER_H

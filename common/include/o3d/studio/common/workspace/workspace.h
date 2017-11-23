/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_WORKSPACE_H
#define _O3DS_COMMON_WORKSPACE_H

#include "../exception.h"
#include "../objectref.h"

#include "project.h"

#include <map>
#include <list>

#include <o3d/core/baseobject.h>
#include <o3d/core/stringlist.h>

namespace o3d {
namespace studio {
namespace common {

class Project;
class Messenger;

/**
 * @brief The Workspace final class.
 */
class O3S_API Workspace : public BaseObject
{
    Q_DECLARE_TR_FUNCTIONS(Workspace)

public:

    Workspace(const String &name, BaseObject *parent = nullptr);
    virtual ~Workspace();

    const Uuid& uuid() const;
    const String& name() const;
    const String& filename() const;

    UInt32 generateProjectId();

    void setUuid(const Uuid &uuid);

    Project* project(const LightRef& ref);
    const Project* project(const LightRef& ref) const;

    Project* activeProject();
    const Project* activeProject() const;

    /**
     * @brief Return the list of found projects.
     * @return
     */
    T_StringList projectsList() const;

    /**
     * @brief loadedProjectList
     * @return
     */
    std::list<Project*> loadedProjectList() const;

    /**
     * @brief addProject
     * @param project
     * @return
     */
    Bool addProject(Project *project);

    /**
     * @brief The project is saved before to be closed and removed from the workspace.
     * @param uuid
     * @return
     */
    Bool closeProject(const LightRef& ref);

    Bool hasProject(const LightRef& ref) const;
    Bool hasProject(String location) const;

    /**
     * @brief Changes occurs to one or more projects.
     * @return
     */
    Bool hasChanges() const;

    /**
     * @brief Set the current selected project.
     * @param name
     */
    Bool setActiveProject(const LightRef &ref);

    Bool save();
    Bool load();

    //
    // Project entity
    //

    /**
     * @brief Get a specific hub from a loaded project.
     */
    Hub* hub(const LightRef &ref);

    /**
     * @brief Get a specific hub from a loaded project (const version).
     */
    const Hub* hub(const LightRef &ref) const;

    /**
     * @brief Find a specific hub, recursively from a loaded project.
     */
    Hub* findHub(const LightRef &ref);

    /**
     * @brief Find a specific hub, recursively from a loaded project (const version).
     */
    const Hub* findHub(const LightRef &ref) const;

    /**
     * @brief Get a specific fragment from a loaded project.
     */
    Fragment* fragment(const LightRef &ref);

    /**
     * @brief Get a specific fragment from a loaded project (const version).
     */
    const Fragment* fragment(const LightRef &ref) const;

    /**
     * @brief Get a specific asset from a loaded project.
     */
    Asset* asset(const LightRef &ref);

    /**
     * @brief Get a specific asset from a loaded project (const version).
     */
    const Asset* asset(const LightRef &ref) const;

public /*signals*/:

    Signal<const LightRef &> onProjectAdded{this};
    Signal<const LightRef &> onProjectActivated{this};
    Signal<const LightRef &> onProjectRemoved{this};

    Signal<const LightRef &> onProjectHubAdded{this};
    Signal<const LightRef &> onProjectHubRemoved{this};

    Signal<const LightRef &> onProjectFragmentAdded{this};
    Signal<const LightRef &> onProjectFragmentRemoved{this};

    Signal<const LightRef &> onProjectAssetAdded{this};
    Signal<const LightRef &> onProjectAssetRemoved{this};

public /*slots*/:

    void onSelectionChanged();

private:

    Uuid m_uuid;
    UInt32 m_nextId;

    String m_filename;        //!< Related workspace file name
    String m_name;            //!< Unique workspace name

    T_StringList m_foundProjects;
    std::map<UInt32, Project*> m_loadedProjects;

    Project *m_activeProject{nullptr};

    Messenger& messenger();
};

/**
 * @brief The WorkspaceException class
 */
class E_WorkspaceException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_WorkspaceException)

    //! Ctor
    E_WorkspaceException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_WorkspaceException, "Objective-3D Studio workspace exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACE_H

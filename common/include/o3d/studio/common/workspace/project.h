/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_PROJECT_H
#define _O3DS_COMMON_PROJECT_H

#include <QtCore/QCoreApplication>

#include <map>
#include <list>

#include "entity.h"

#include "../exception.h"
#include "../objectref.h"

#include <o3d/core/localdir.h>

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class MasterScene;
class ProjectFile;
class ProjectInfo;
class Hub;
class Fragment;
class RootHub;

/**
 * @brief The Project final class
 */
class O3S_API Project : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Project)

    friend class ProjectFile;

public:

    Project(const String &name, Workspace *workspace = nullptr);
    virtual ~Project();

    virtual Project* project() override;
    virtual const Project* project() const override;

    void setWorkspace(Workspace *workspace);

    Workspace* workspace();
    const Workspace* workspace() const;

    UInt64 generateEntityId();

    virtual Int32 childIndexOf(Entity *entity) const override;

    /**
     * @brief Has changes to saved since last save(). Project change or any of its child entity.
     * If the project itself contains changes, it directly returns true.
     * Else it perform a check on throught the lookup maps of each child and returns on the first
     * entity having changes.
     */
    virtual Bool hasChanges() const override;

    /**
     * @brief Initialize a new project at the specified path with name.
     * @param stream
     */
    virtual void create() override;
    virtual Bool deletable() const override;

    String filename() const;
    const LocalDir& path() const;

    Bool setLocation(const LocalDir &path);

    const ProjectInfo& info() const;
    ProjectInfo& info();

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    void setupMasterScene();

    //
    // entity
    //

    /**
     * @brief Search in the global map of entity by its light ref (faster than recursive).
     */
    Entity* lookup(const LightRef &ref);

    /**
     * @brief Search in the global map of entity by its light ref (faster than recursive)
     * (const version).
     */
    const Entity* lookup(const LightRef &ref) const;

    /**
     * @brief Search in the global map of entity by its UUID (faster than recursive).
     */
    Entity* lookup(const Uuid &uuid);

    /**
     * @brief Search in the global map of entity by its UUID (faster than recursive)
     * (const version).
     */
    const Entity* lookup(const Uuid &uuid) const;

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    void purgeEntities();

    //
    // hub
    //

    /**
     * @brief Return the virtual root hub.
     */
    Hub* rootHub();

    /**
     * @brief Return the virtual root hub (read only)
     */
    Hub* rootHub() const;

    //
    // fragment
    //

    void addFragment(Fragment *fragment);

    void removeFragment(const LightRef &ref);
    void removeFragment(UInt64 id);
    void removeFragment(Fragment *fragment);

    Fragment* fragment(const LightRef &ref);
    const Fragment* fragment(const LightRef &ref) const;

    Fragment* fragment(UInt64 id);
    const Fragment* fragment(UInt64 id) const;

    std::list<Fragment*> searchFragment(const String &name);
    std::list<const Fragment*> searchFragment(const String &name) const;

    std::list<Fragment*> fragments();
    std::list<const Fragment*> fragments() const;

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    String m_filename;         //!< Project file name
    LocalDir m_path;            //!< Project path

    ProjectFile *m_projectFile;

    UInt64 m_nextId;

    ProjectInfo *m_info;
    MasterScene *m_masterScene;

    std::map<UInt64, Fragment*> m_fragments;   //!< Children fragment.

    RootHub *m_rootHub;

    //! Global map by UUID
    std::map<Uuid, Entity*> m_entitiesByUuid;
    //! Global map by ID
    std::map<UInt64, Entity*> m_entitiesById;

    //! Contains the entities to later at a safe time
    std::list<Entity*> m_deferredDelete;
};

/**
 * @brief The ProjectException class
 */
class E_ProjectException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_ProjectException)

    //! Ctor
    E_ProjectException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_ProjectException, "Objective-3D Studio project exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

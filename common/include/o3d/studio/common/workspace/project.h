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

#include <o3d/core/diskdir.h>

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class MasterScene;
class O3DCanvasContent;
class ProjectFile;
class ProjectInfo;
class Hub;
class Fragment;
class Asset;

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

    String filename() const;
    const DiskDir& path() const;

    Bool setLocation(const DiskDir &path);

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
    void removeEntity(const ObjectRef &ref);

    //
    // hub
    //

    void addHub(Hub *hub, Int32 index = -1);

    void removeHub(const LightRef &ref);
    void removeHub(UInt64 id);
    void removeHub(Hub *hub);

    Hub* hub(const LightRef &ref);
    const Hub* hub(const LightRef &ref) const;

    Hub* hub(UInt64 id);
    const Hub* hub(UInt64 id) const;

    std::list<Hub*> searchHub(const String &name);
    std::list<const Hub*> searchHub(const String &name) const;

    /**
     * @brief Recursively find for a hub instance.
     */
    Hub* findHub(UInt64 id);

    /**
     * @brief Recursively find for a hub instance (const version).
     */
    const Hub* findHub(UInt64 id) const;

    /**
     * @brief Recursively find for a hub instance.
     */
    Hub* findHub(const Uuid &uuid);

    /**
     * @brief Recursively find for a hub instance (const version).
     */
    const Hub* findHub(const Uuid &uuid) const;


    size_t numHubs() const;

    /**
     * @brief List of hubs of the project
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    std::list<Hub*> hubs(Bool recurse = false);

    /**
     * @brief List of hubs of the project (const version).
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    std::list<const Hub*> hubs(Bool recurse = false) const;

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

    //
    // asset
    //

    void addAsset(Asset *asset);

    void removeAsset(const LightRef &ref);
    void removeAsset(UInt64 id);
    void removeAsset(Asset *asset);

    Asset* asset(const LightRef &ref);
    const Asset* asset(const LightRef &ref) const;

    Asset* asset(UInt64 id);
    const Asset* asset(UInt64 id) const;

    std::list<Asset*> searchAsset(const String &name);
    std::list<const Asset*> searchAsset(const String &name) const;

    std::list<Asset*> assets();
    std::list<const Asset*> assets() const;

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    String m_filename;         //!< Project file name
    DiskDir m_path;            //!< Project path

    ProjectFile *m_projectFile;

    UInt64 m_nextId;

    ProjectInfo *m_info;
    MasterScene *m_masterScene;

    std::map<UInt64, Fragment*> m_fragments;   //!< Children fragment.
    std::map<UInt64, Hub*> m_hubs;             //!< Children hubs (direct).
    std::map<UInt64, Asset*> m_assets;         //!< Children assets.

    //! Global map by UUID
    std::map<Uuid, Entity*> m_entitiesByUuid;
    //! Global map by ID
    std::map<UInt64, Entity*> m_entitiesById;

    std::list<UInt64> m_hubsOrder;             //!< Hub children ordering
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

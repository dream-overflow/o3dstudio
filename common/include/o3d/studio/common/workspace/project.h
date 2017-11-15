/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_PROJECT_H
#define _O3DS_COMMON_PROJECT_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "entity.h"

#include "../exception.h"
#include "../objectref.h"

class QDataStream;

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

    Project(const QString &name, Workspace *workspace = nullptr);
    virtual ~Project();

    virtual Project* project() override;
    virtual const Project* project() const override;

    void setWorkspace(Workspace *workspace);

    Workspace* workspace();
    const Workspace* workspace() const;

    quint64 generateEntityId();

    /**
     * @brief Has changes to saved since last save(). Project change or any of its child entity.
     * If the project itself contains changes, it directly returns true.
     * Else it perform a check on throught the lookup maps of each child and returns on the first
     * entity having changes.
     */
    virtual bool hasChanges() const override;

    /**
     * @brief Initialize a new project at the specified path with name.
     * @param stream
     */
    virtual void create() override;

    QString filename() const;
    const QDir& path() const;

    bool setLocation(const QDir &path);

    const ProjectInfo& info() const;
    ProjectInfo& info();

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

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
    Entity* lookup(const QUuid &uuid);

    /**
     * @brief Search in the global map of entity by its UUID (faster than recursive)
     * (const version).
     */
    const Entity* lookup(const QUuid &uuid) const;


    //
    // hub
    //

    void addHub(Hub *hub);

    void removeHub(const LightRef &ref);
    void removeHub(qint64 id);
    void removeHub(Hub *hub);

    Hub* hub(const LightRef &ref);
    const Hub* hub(const LightRef &ref) const;

    Hub* hub(qint64 id);
    const Hub* hub(qint64 id) const;

    QList<Hub*> searchHub(const QString &name);
    QList<const Hub*> searchHub(const QString &name) const;

    /**
     * @brief Recursively find for a hub instance.
     */
    Hub* findHub(quint64 id);

    /**
     * @brief Recursively find for a hub instance (const version).
     */
    const Hub* findHub(quint64 id) const;

    /**
     * @brief Recursively find for a hub instance.
     */
    Hub* findHub(const QUuid &uuid);

    /**
     * @brief Recursively find for a hub instance (const version).
     */
    const Hub* findHub(const QUuid &uuid) const;


    int numHubs() const;

    /**
     * @brief List of hubs of the project
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    QList<Hub*> hubs(bool recurse = false);

    /**
     * @brief List of hubs of the project (const version).
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    QList<const Hub*> hubs(bool recurse = false) const;

    //
    // fragment
    //

    void addFragment(Fragment *fragment);

    void removeFragment(const LightRef &ref);
    void removeFragment(quint64 id);
    void removeFragment(Fragment *fragment);

    Fragment* fragment(const LightRef &ref);
    const Fragment* fragment(const LightRef &ref) const;

    Fragment* fragment(quint64 id);
    const Fragment* fragment(quint64 id) const;

    QList<Fragment*> searchFragment(const QString &name);
    QList<const Fragment*> searchFragment(const QString &name) const;

    QList<Fragment*> fragments();
    QList<const Fragment*> fragments() const;

    //
    // asset
    //

    void addAsset(Asset *asset);

    void removeAsset(const LightRef &ref);
    void removeAsset(quint64 id);
    void removeAsset(Asset *asset);

    Asset* asset(const LightRef &ref);
    const Asset* asset(const LightRef &ref) const;

    Asset* asset(quint64 id);
    const Asset* asset(quint64 id) const;

    QList<Asset*> searchAsset(const QString &name);
    QList<const Asset*> searchAsset(const QString &name) const;

    QList<Asset*> assets();
    QList<const Asset*> assets() const;

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    QString m_filename;        //!< Project file name
    QDir m_path;               //!< Project path

    ProjectFile *m_projectFile;

    quint64 m_nextId;

    ProjectInfo *m_info;
    MasterScene *m_masterScene;

    QMap<quint64, Fragment*> m_fragments;   //!< Children fragment.
    QMap<quint64, Hub*> m_hubs;             //!< Children hubs (direct).
    QMap<quint64, Asset*> m_assets;         //!< Children assets.

    //! Global map by UUID
    QMap<QUuid, Entity*> m_entitiesByUuid;
    //! Global map by ID
    QMap<quint64, Entity*> m_entitiesById;
};

/**
 * @brief The ProjectException class
 */
class ProjectException : public BaseException
{
public:

    ProjectException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

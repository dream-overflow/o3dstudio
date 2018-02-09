/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_HUB_H
#define _O3DS_COMMON_HUB_H

#include <QtCore/QCoreApplication>

#include <map>
#include <list>

#include "../global.h"
#include "../exception.h"
#include "../objectref.h"

#include "entity.h"

namespace o3d {
namespace studio {
namespace common {

class Fragment;
class MasterScene;

/**
 * @brief The Hub base class
 */
class O3S_API Hub : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Hub)

public:

    explicit Hub(const String &name, Entity *parent = nullptr);
    virtual ~Hub();

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    virtual void create() override;
    virtual void destroy() override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;
    virtual Bool deletable() const override;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

    //
    // children hub
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

    /**
     * @brief Return the number of direct hub children.
     */
    size_t numHubs() const;

    /**
     * @brief List of children hubs.
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    std::list<Hub*> hubs(Bool recurse = False);

    /**
     * @brief List of children hubs (const version).
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    std::list<const Hub*> hubs(Bool recurse = False) const;

    virtual Int32 childIndexOf(Entity *entity) const override;

    //
    // engine proxy
    //

    /**
     * @brief Create the related entities into the engine scene.
     */
    virtual void createToScene(MasterScene *masterScene) = 0;

    /**
     * @brief Destroy the related entities from the engine scene.
     * @note This default implemantation process recursively,
     * and starting by removing the leaves throught, and up to this hub.
     * This default implementation can be used at begining of each specialization.
     */
    virtual void removeFromScene(MasterScene *masterScene);

    /**
     * @brief Bi-directionnal synchronization with the engine scene.
     */
    virtual void syncWithScene(MasterScene *masterScene) = 0;

protected:

    Project *m_project;               //!< Owner project;

    std::map<UInt64, Hub*> m_hubs;    //!< Child hubs
    std::list<UInt64> m_hubsOrder;    //!< Children ordering
};

/**
 * @brief The HubException class
 */
class E_HubException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_HubException)

    //! Ctor
    E_HubException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_HubException, "Objective-3D Studio hub exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_HUB_H

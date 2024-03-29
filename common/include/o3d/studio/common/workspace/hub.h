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

class Matrix4;

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

    virtual Role role() const override;

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    /**
     * @brief Create the hub and manage scene creation command.
     */
    virtual void create() override;

    /**
     * @brief Create the hub and manage scene creation command.
     */
    virtual void destroy() override;

    /**
     * @brief Create the hub and manage scene creation command.
     */
    virtual void update() override;

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
     * @param recurse Default false, returns only first level hubs, true recurse over all children.
     */
    std::list<Hub*> hubs(Bool recurse = False);

    /**
     * @brief List of children hubs (const version).
     * @param recurse Default false, returns only first level hubs, true recurse over all children.
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
     */
    virtual void removeFromScene(MasterScene *masterScene) = 0;

    /**
     * @brief Bi-directionnal synchronization with the engine scene.
     */
    virtual void syncWithScene(MasterScene *masterScene) = 0;

    //
    // transforms
    //

    /**
     * Return the absolute transform matrix (read only).
     * @note Default returns an identity matrix or the parent absolute matrix if it is a
     * spacial node.
     */
    virtual const Matrix4& absoluteMatrix(MasterScene *masterScene) const;

    /**
     * @brief Return true if the node contains spacial information (transforms...).
     */
    virtual Bool isSpacialNode() const;

    /**
     * @brief Is parent node is a spacial node.
     */
    Bool isParentSpacialNode() const;

    //
    // UI
    //

    /**
     * @brief Set focus state for a particular master scene.
     */
    virtual void setFocus(MasterScene *masterScene, Bool focus);

    /**
     * @brief Is the hub has focus for a specific master scene.
     */
    virtual Bool hasFocus(MasterScene *masterScene);

    /**
     * @brief Set hover state for a particular master scene.
     */
    virtual void setHover(MasterScene *masterScene, Bool hover);

    /**
     * @brief Is the hub is hovered for a specific master scene.
     */
    Bool isHover(MasterScene *masterScene);

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

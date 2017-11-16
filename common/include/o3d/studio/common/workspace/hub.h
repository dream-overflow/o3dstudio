/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_HUB_H
#define _O3DS_COMMON_HUB_H

#include <QtCore/QMap>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

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

    explicit Hub(const QString &name, Entity *parent = nullptr);
    virtual ~Hub();

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

    //
    // children hub
    //

    void addHub(Hub *hub);

    void removeHub(const LightRef &ref);
    void removeHub(quint64 id);
    void removeHub(Hub *hub);

    Hub* hub(const LightRef &ref);
    const Hub* hub(const LightRef &ref) const;

    Hub* hub(quint64 id);
    const Hub* hub(quint64 id) const;

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
     * @brief List of children hubs.
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    QList<Hub*> hubs(bool recurse = false);

    /**
     * @brief List of children hubs (const version).
     * @param recurse Default false, returns only projet level hubs, true recurse over all children.
     */
    QList<const Hub*> hubs(bool recurse = false) const;

    //
    // engine proxy
    //

    /**
     * @brief Create the related entities into the engine scene.
     */
    virtual void createToScene(MasterScene *masterScene) = 0;

    /**
     * @brief Destroy the related entities fomr the engine scene.
     */
    virtual void removeFromScene(MasterScene *masterScene) = 0;

    /**
     * @brief Bi-directionnal synchronization with the engine scene.
     */
    virtual void syncWithScene(MasterScene *masterScene) = 0;

private:

    Project *m_project;            //!< Owner project;

    QMap<quint64, Hub*> m_hubs;    //!< Child hubs
};

/**
 * @brief The HubException class
 */
class HubException : public BaseException
{
public:

    HubException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_HUB_H

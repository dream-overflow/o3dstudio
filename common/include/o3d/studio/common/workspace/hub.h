/**
 * @brief Common hub object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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

    Project* project();
    const Project* project() const;

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;
    virtual bool hasChanges() override;

    //
    // children hub
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
    Hub* findHub(qint64 id);

    /**
     * @brief Recursively find for a hub instance (const version).
     */
    const Hub* findHub(qint64 id) const;

    int numHubs() const;

private:

    Project *m_project;         //!< Owner project;

    QMap<qint64, Hub*> m_hubs;  //!< Child hubs
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

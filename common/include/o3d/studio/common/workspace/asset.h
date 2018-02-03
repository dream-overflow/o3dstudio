/**
 * @brief Common asset object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#ifndef _O3DS_COMMON_ASSET_H
#define _O3DS_COMMON_ASSET_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "../exception.h"
#include "../objectref.h"

#include <o3d/core/datetime.h>

#include "entity.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
class Hub;

/**
 * @brief The Asset base class
 * @todo no asset in projet, but specific project export/import sub project
 */
class O3S_API Asset : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Asset)

public:

    enum VersionStatus
    {
        PROTO_VERSION,
        ALPHA_VERSION,
        NIGHTLY_VERSION,
        BETA_VERSION,
        RC_VERSION,
        STABLE_VERSION
    };

    Asset(const String &name, Entity *parent = nullptr);
    virtual ~Asset();

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    virtual Int32 childIndexOf(Entity *entity) const override;

    virtual void create() override;
    virtual Bool deletable() const override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

    void setCreator(const String &creator);
    const String& creator() const;

    void setDescription(const String &descr);
    const String& description() const;

    void setComment(const String &comment);
    const String& comment();

    void setVersion(UInt32 majorVers, UInt32 minorVers, UInt32 subVers);
    UInt32 version(UInt32 part) const;

    void setVersionStatus(VersionStatus status);
    VersionStatus versionStatus() const;

    void setRevision(UInt32 rev);
    UInt32 revision() const;

    void setCreationTimeStamp(const DateTime &date);
    const DateTime& creationTimeStamp() const;

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

    /**
     * @brief Return the number of direct hub children.
     */
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

private:

    String m_creator;
    String m_description;
    String m_comment;

    UInt32 m_version[3];            //!< major, minor, sub-minor
    VersionStatus m_versionStatus;  //!< version status (alpha, beta... stable)...
    UInt32 m_revision;              //!< revision number (counter)

    DateTime m_creationTimeStamp;

    std::map<UInt64, Hub*> m_hubs;    //!< Child hubs
    std::list<UInt64> m_hubsOrder;    //!< Children ordering
};

/**
 * @brief The AssetException class
 */
class E_AssetException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_AssetException)

    //! Ctor
    E_AssetException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_AssetException, "Objective-3D Studio asset exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ASSET_H

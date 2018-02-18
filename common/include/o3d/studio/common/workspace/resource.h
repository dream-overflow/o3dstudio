/**
 * @brief Common resource object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-16
 * @details
 */

#ifndef _O3DS_COMMON_RESOURCE_H
#define _O3DS_COMMON_RESOURCE_H

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

class MasterScene;

/**
 * @brief The Resource base class
 */
class O3S_API Resource : public Entity
{
    Q_DECLARE_TR_FUNCTIONS(Resource)

public:

    explicit Resource(const String &name, Entity *parent = nullptr);
    virtual ~Resource();

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

    virtual Int32 childIndexOf(Entity *entity) const override;

    //
    // engine proxy
    //

//    /**
//     * @brief Create the related entities into the engine scene.
//     */
//    virtual void createToScene(MasterScene *masterScene) = 0;

//    /**
//     * @brief Destroy the related entities from the engine scene.
//     * @todo should not be recusrive, only for himself...
//     * @note This default implemantation process recursively,
//     * and starting by removing the leaves throught, and up to this hub.
//     * This default implementation can be used at begining of each specialization.
//     */
//    virtual void removeFromScene(MasterScene *masterScene);

//    /**
//     * @brief Bi-directionnal synchronization with the engine scene.
//     */
//    virtual void syncWithScene(MasterScene *masterScene) = 0;

protected:

    Project *m_project;               //!< Owner project;
};

/**
 * @brief The ResourceException class
 */
class E_ResourceException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_ResourceException)

    //! Ctor
    E_ResourceException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_ResourceException, "Objective-3D Studio resource exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_RESOURCE_H

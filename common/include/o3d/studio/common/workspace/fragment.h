/**
 * @brief Common fragment object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_FRAGMENT_H
#define _O3DS_COMMON_FRAGMENT_H

#include "../exception.h"
#include "../objectref.h"

#include "entity.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
class Hub;
class MasterScene;

/**
 * @brief The Fragment base class
 */
class O3S_API Fragment : public Entity
{
public:

    Fragment(const String &name, Entity *parent = nullptr);
    virtual ~Fragment();

    virtual Role role() const override;

    void setProject(Project *project);

    virtual Project* project() override;
    virtual const Project* project() const override;

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    virtual Int32 childIndexOf(Entity *entity) const override;

    void setupMasterScene();

    virtual void create() override;
    virtual void destroy() override;
    virtual void update() override;

    virtual Bool deletable() const override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    void linkToHub(Hub *hub);

    Hub* hub();
    const Hub* hub() const;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

private:

    ObjectRef m_hubRef;     //! Related hub ref
    Hub *m_hub;             //! Related hub

    MasterScene *m_masterScene;
};

/**
 * @brief The FragmentException class
 */
class E_FragmentException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_FragmentException)

    //! Ctor
    E_FragmentException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_FragmentException, "Objective-3D Studio fragment exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_FRAGMENT_H

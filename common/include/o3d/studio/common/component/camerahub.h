/**
 * @brief Common camera component offering a camera hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#ifndef _O3DS_COMMON_CAMERAHUB_H
#define _O3DS_COMMON_CAMERAHUB_H

#include "component.h"
#include "../workspace/hub.h"

#include <map>

namespace o3d {

class Camera;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

/**
 * @brief The CameraComponent class
 */
class O3S_API CameraComponent : public Component
{
public:

    CameraComponent();
    virtual ~CameraComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The CameraHub class
 */
class O3S_API CameraHub : public Hub
{
public:

    enum CameraMode
    {
        CAMERA_ORTHO = 0,
        CAMERA_ORTHO_BY_FOV = 1,
        CAMERA_PERSPECTIVE = 2
    };

    explicit CameraHub(const String &name, Entity *parent = nullptr);
    virtual ~CameraHub();

    virtual void create() override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const;
    virtual Bool deserializeContent(InStream &stream);

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

    //
    // Properties
    //

    // @todo

protected:

    Float m_left, m_right;   //!< Orthonormal 2d projection coordinates
    Float m_bottom,m_top;    //!< Orthonormal 2d projection coordinates

    Float m_coef;            //!< reshape width/height
    Float m_fov;             //!< half field of view in degree (60 by default)

    Float m_znear, m_zfar;   //!< Depth clipping plane

    CameraMode m_cameraMode;

    std::map<MasterScene*, o3d::Camera*> m_instances;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CAMERAHUB_H

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
    virtual Hub* buildHub(const QString &name, Project *project, Entity *parent) override;

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

    explicit CameraHub(const QString &name, Entity *parent = nullptr);
    virtual ~CameraHub();

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

    //
    // Properties
    //

    // @todo

protected:

    float m_left, m_right;   //!< Orthonormal 2d projection coordinates
    float m_bottom,m_top;    //!< Orthonormal 2d projection coordinates

    float m_coef;            //!< reshape width/height
    float m_fov;             //!< half field of view in degree (60 by default)

    float m_znear, m_zfar;   //!< Depth clipping plane

    CameraMode m_cameraMode;

    QMap<MasterScene*, o3d::Camera*> m_instances;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CAMERAHUB_H

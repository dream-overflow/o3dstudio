/**
 * @brief Common viewport component offering a viewport hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#ifndef _O3DS_COMMON_VIEWPORTHUB_H
#define _O3DS_COMMON_VIEWPORTHUB_H

#include "component.h"
#include "../workspace/hub.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;
class Project;
class CameraHub;
class DrawerHub;

/**
 * @brief The ViewPortComponent class
 */
class O3S_API ViewPortComponent : public Component
{
public:

    ViewPortComponent();
    virtual ~ViewPortComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const QString &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The ViewPortHub class
 */
class O3S_API ViewPortHub : public Hub
{
public:

    enum ViewPortMode
    {
        VIEWPORT_SCREEN = 0,    //!< Viewport redirecting to the screen.
        VIEWPORT_FEEDBACK = 1,  //!< Viewport redirecting to a off-screen buffer.
    };

    enum ViewPortImpl
    {
        VIEWPORT_ENGINE = 0,   //!< Use the engine management for the viewport
        VIEWPORT_NATIVE = 1    //!< Use the capacity of the material to managed viewport in addition of the engine capacity
    }

    explicit ViewPortHub(const QString &name, Entity *parent = nullptr);
    virtual ~ViewPortHub();

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

    qint32 m_priority;

    float m_xpos, m_ypos;        //!< position
    float m_width, m_height;     //!< size (can be in percent)

    bool m_percent;              //!< is using size in percent
    bool m_isActive;             //!< is active

    qint32 m_nWidth, m_nHeight;  //!< real view-port size

    CameraHub *m_cameraHub;
    DrawerHub *m_drawerHub;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VIEWPORTHUB_H

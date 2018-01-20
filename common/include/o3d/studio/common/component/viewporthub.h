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
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

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
    };

    explicit ViewPortHub(const String &name, Entity *parent = nullptr);
    virtual ~ViewPortHub();

    virtual void create() override;
    virtual Bool deletable() const override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

    //
    // Properties
    //

    // @todo

protected:

    Int32 m_priority;

    Float m_xpos, m_ypos;        //!< position
    Float m_width, m_height;     //!< size (can be in percent)

    Bool m_percent;              //!< is using size in percent
    Bool m_isActive;             //!< is active

    Int32 m_nWidth, m_nHeight;  //!< real view-port size

    CameraHub *m_camera;
    DrawerHub *m_drawer;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VIEWPORTHUB_H

/**
 * @brief Common UI scene camera manipulator view
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @details
 */

#ifndef _O3DS_CAMERAMANIPULATOR_H
#define _O3DS_CAMERAMANIPULATOR_H

#include "sceneuielement.h"

#include <o3d/core/vector2.h>
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The working camera helper attached to each master scene.
 */
class O3S_API CameraManipulator : public SceneUIElement
{
public:

    enum Part
    {
        PART_NONE = 0,
        PART_CUBE = 0xffffff04,
        PART_X_PLUS,
        PART_X_MINUS,
        PART_Y_PLUS,
        PART_Y_MINUS,
        PART_Z_PLUS,
        PART_Z_MINUS
    };

    enum Action
    {
        ACTION_NONE = 0,
        ACTION_TRANSLATION,
        ACTION_ROTATION,
        ACTION_ZOOM
    };

    enum CameraView
    {
        VIEW_ANY = 0,
        VIEW_LEFT,
        VIEW_RIGHT,
        VIEW_TOP,
        VIEW_BOTTOM,
        VIEW_FRONT,
        VIEW_BACK
    };

    enum CameraMode
    {
        PERSPECTIVE = 0,
        ORTHO,
        LEFT_ORTHO,
        RIGHT_ORTHO,
        TOP_ORTHO,
        BOTTOM_ORTHO,
        FRONT_ORTHO,
        BACK_ORTHO
    };

    /**
     * @brief CameraManipulator
     * @param parent Parent object
     * @param pos Position in percent of the display
     * @param scale Scale of the object (default 1.0)
     */
    CameraManipulator(BaseObject *parent, const Point2f &pos, const Float scale = 1.f);
    virtual ~CameraManipulator();

    virtual void setup(MasterScene *masterScene) override;
    virtual void release(MasterScene *masterScene) override;

    virtual void hover(UInt32 id, const Point3f &pos) override;
    virtual void leave() override;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;
    virtual void directRendering(DrawInfo &drawInfo, MasterScene *masterScene) override;

    virtual Bool keyPressEvent(const KeyEvent &event, MasterScene *masterScene) override;
    virtual Bool keyReleaseEvent(const KeyEvent &event, MasterScene *masterScene) override;
    virtual Bool mousePressEvent(const MouseEvent &event, MasterScene *masterScene) override;
    virtual Bool mouseReleaseEvent(const MouseEvent &event, MasterScene *masterScene) override;
    virtual Bool mouseMoveEvent(const MouseEvent &event, MasterScene *masterScene) override;
    virtual Bool wheelEvent(const WheelEvent &event, MasterScene *masterScene) override;

    virtual Bool mouseDoubleClickEvent(const MouseEvent &event, MasterScene *masterScene) override;

    CameraMode cameraMode() const;
    CameraView cameraView() const;

    void reshape(MasterScene *masterScene, const Vector2i &size);

protected:

    Point2f m_pos;
    Float m_scale;

    Part m_hoverPart;
    Part m_activePart;

    Action m_action;

    CameraMode m_cameraMode;
    CameraView m_cameraView;

    void setOrtho(MasterScene *masterScene);
    void setPerspective(MasterScene *masterScene);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_CAMERAMANIPULATOR_H

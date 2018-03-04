/**
 * @brief Workspace master scene rendering and management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_MASTERSCENE_H
#define _O3DS_COMMON_MASTERSCENE_H

#include <list>
#include <unordered_map>

#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/ui/canvas/o3ddrawer.h"

#include <o3d/core/baseobject.h>
#include <o3d/core/smartobject.h>
#include <o3d/core/vector3.h>

namespace o3d {

class Scene;
class Camera;
class ViewPort;
class SceneDrawer;
class Renderer;
class Pickable;

namespace studio {
namespace common {

class O3DCanvasContent;
class SceneCommand;
class SceneUIElement;
class CameraManipulator;
class HubManipulator;

class Event;
class KeyEvent;
class MouseEvent;
class FocusEvent;
class MasterSceneDrawer;

class O3S_API MasterScene : public BaseObject, public O3DDrawer
{
public:

    enum ActionMode
    {
        ACTION_NONE = 0,
        ACTION_CAMERA_ZOOM,
        ACTION_CAMERA_TRANSLATION,
        ACTION_CAMERA_ROTATION,
        ACTION_SELECTION,
        ACTION_TRANSLATION,
        ACTION_ROTATION,
        ACTION_SCALE,
        ACTION_SKEW
    };

    enum MotionType
    {
        MOTION_FOLLOW = 0,  //!< follow the cursor
        MOTION_PRECISE,     //!< scale 0.1
        MOTION_FAST,        //!< scale 10
        MOTION_STEP,        //!< follow an increment step
        MOTION_GRID,        //!< follow a grid as magnet
        MOTION_MAGNET       //!< follow a constraint of magnet with others objects
    };

    MasterScene(Entity *parent);
    virtual ~MasterScene();

    virtual void initialize(Bool debug = False);

    const Entity* parent() const;
    Entity* parent();

    const Project* project() const;
    Project* project();

    Renderer* renderer();
    O3DCanvasContent* widget();

    o3d::Scene* scene();
    const o3d::Scene* scene() const;

    O3DCanvasContent *content();
    const O3DCanvasContent *content() const;

    Hub *hoverHub();
    const Hub *hoverHub() const;

    //
    // commands
    //

    void addCommand(SceneCommand *command);

    //
    // scene UI elements
    //

    void addSceneUIElement(SceneUIElement *elt);
    void removeSceneUIElement(SceneUIElement *elt);

    // @todo void setHubManipulatorComponent(Component *component);

    /**
     * @brief Get current hub manipulator.
     */
    SceneUIElement* hubManipulator();

    /**
     * @brief Get current hover scene UI element.
     */
    SceneUIElement* hoverSceneUIElement();

    //
    // drawer
    //

    virtual void initializeDrawer() override;
    virtual void paintDrawer() override;
    virtual void updateDrawer() override;
    virtual void resizeDrawer(int w, int h) override;
    virtual void terminateDrawer() override;

    virtual Bool mousePressEvent(const MouseEvent &event) override;
    virtual Bool mouseReleaseEvent(const MouseEvent &event) override;
    virtual Bool mouseDoubleClickEvent(const MouseEvent &event) override;
    virtual Bool mouseMoveEvent(const MouseEvent &event) override;
    virtual Bool wheelEvent(const WheelEvent &event) override;

    virtual Bool keyPressEvent(const KeyEvent &event) override;
    virtual Bool keyReleaseEvent(const KeyEvent &event) override;

    virtual Bool focusInEvent(const Event &event) override;
    virtual Bool focusOutEvent(const Event &event) override;

    virtual Bool enterEvent(const Event &event) override;
    virtual Bool leaveEvent(const Event &event) override;

    //
    // miscs
    //

    void updateCounters(UInt32 pass);

    UInt32 numVertices(UInt32 pass) const;
    UInt32 numTriangles(UInt32 pass) const;
    UInt32 numLines(UInt32 pass) const;
    UInt32 numPoints(UInt32 pass) const;

    //
    // manipulation
    //

    /**
     * @brief Current action mode.
     */
    ActionMode actionMode() const;

    /**
     * @brief Motion type related to the current action mode.
     */
    MotionType motionType() const;

    /**
     * @brief Map a picking id to a scene UI element.
     * @param id Unique picking id.
     * @param element Instance of the scene UI element.
     */
    void registerPickingId(UInt32 id, SceneUIElement *element);

    /**
     * @brief Unregister a picking id.
     */
    void unregisterPickingId(UInt32 id);

public /*slots*/:

    void pickingHit(Pickable* pickable, Vector3 pos);
    void elementPickingHit(UInt32 id, Vector3 pos);
    void pickingNoHit();

    void onSelectionChanged();

private:

    Entity *m_parent;             //!< Parent entity (project, fragment)

    O3DCanvasContent *m_content;  //!< Attached widget
    o3d::Renderer *m_renderer;    //!< Attached renderer
    o3d::Scene *m_scene;          //!< Related o3d scene

    Point2i m_lockedPos;          //!< Infinite cursor
    ActionMode m_actionMode;      //!< Current action mode
    MotionType m_motionType;      //!< Current motion type

    UInt32 m_verticesCount[4];
    UInt32 m_trianglesCount[4];
    UInt32 m_linesCount[4];
    UInt32 m_pointCount[4];

    Hub *m_hoverHub;              //!< Current hub hovered by cursor
    o3d::Point3f m_pickPos;       //!< Current picking position

    //! Main working camera, cannot be deleted
    o3d::SmartObject<o3d::Camera> m_camera;

    //! Main working viewport, cannot be deleted
    o3d::SmartObject<o3d::ViewPort> m_viewport;

    //! Main working drawer, cannot be deleted
    o3d::SmartObject<MasterSceneDrawer> m_sceneDrawer;

    //! Ordered list of command to process during the moment where the context is set to current.
    //! In others words it is the pass of synchronization that will be executed here.
    std::list<SceneCommand*> m_commands;

    //! Attached scene UI elements
    std::list<SceneUIElement*> m_sceneUIElements;

    CameraManipulator *m_cameraManipulator;  //!< Active camera manipulator.
    HubManipulator *m_hubManipulator;        //!< Current hub selection manipulator.
    SceneUIElement *m_hoverUIElement;        //!< Current hover ui element.

    //! Mapping of picking id to scene UI elements (can have multiple picking id to a same element)
    std::unordered_map<UInt32, SceneUIElement*> m_pickingToSceneUIElements;

    void processCommands();
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENE_H

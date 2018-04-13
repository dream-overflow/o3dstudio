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
class Transform;

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
        ACTION_TRANSFORM
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

    /**
     * @brief Get current hover scene UI element (read-only).
     */
    const SceneUIElement* hoverSceneUIElement() const;

    /**
     * @brief Query acquire focus for a specific ui element.
     * @return True if focus is aquired
     */
    Bool acquireFocus(SceneUIElement *uiElement);

    /**
     * @brief Get current focused scene UI element.
     */
    SceneUIElement* focusSceneUIElement();

    /**
     * @brief Get current focused scene UI element (read-only).
     */
    const SceneUIElement* focusSceneUIElement() const;

    /**
     * @brief Get the active camera of the master scene (read-only).
     * @todo could use the CameraManipulator.
     */
    const Camera* camera() const;

    /**
     * @brief Get the active camera of the master scene.
     * @todo could use the CameraManipulator.
     */
    Camera* camera();

    /**
     * @brief Get the camera manipulator of the master scene (read-only).
     */
    const CameraManipulator* cameraManipulator() const;

    /**
     * @brief Get the camera manipulator of the master scene.
     */
    CameraManipulator* cameraManipulator();

    /**
     * @brief Related viewport area.
     */
    Box2i viewPort() const;

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
     * @brief Set current action mode
     */
    void setActionMode(ActionMode actionMode);

    /**
     * @brief Current action mode.
     */
    ActionMode actionMode() const;

    /**
     * @brief Set current motion type.
     */
    void setMotionType(MotionType motionType);

    /**
     * @brief Motion type related to the current action mode.
     */
    MotionType motionType() const;

    /**
     * @brief Selected transform mode.
     */
    Int32 transformMode() const;

    /**
     * @brief Get the last delta transform during a move event.
     */
    const Vector3f &transformDelta() const;

    /**
     * @brief Get the duration of the last frame in seconds.
     */
    Float frameDuration() const;

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

    void onProjectHubRemoved(LightRef ref);
    void onProjectEntityChanged(LightRef ref, BitSet64 changes);

    void changeTransformMode(Int32 mode);
    void changeTransformOrientation(Int32 mode);
    void changePivotMode(Int32 mode);

private:

    Entity *m_parent;             //!< Parent entity (project, fragment)

    O3DCanvasContent *m_content;  //!< Attached widget
    o3d::Renderer *m_renderer;    //!< Attached renderer
    o3d::Scene *m_scene;          //!< Related o3d scene

    Point2i m_lockedPos;          //!< Infinite cursor
    ActionMode m_actionMode;      //!< Current action mode
    ActionMode m_prevActionMode;  //!< Previous action mode
    MotionType m_motionType;      //!< Current motion type

    Int32 m_transformMode;        //!< Selected transform mode
    Int32 m_pivotMode;            //!< Selected pivot mode
    Int32 m_orientationMode;      //!< Selected orientation mode

    UInt32 m_verticesCount[4];
    UInt32 m_trianglesCount[4];
    UInt32 m_linesCount[4];
    UInt32 m_pointCount[4];

    o3d::Point3f m_pointerPos;    //!< Primary pointer position (generally the 2d mouse cursor)

    Hub *m_hoverHub;              //!< Current hub hovered by cursor
    o3d::Point3f m_pickPos;       //!< Current picking position

    Vector3f m_transformDelta;    //!< Transform delta of velocity

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

    UInt32 m_hoverId;  //!< Unique picking id of the current hover object.

    SceneUIElement *m_hoverUIElement;  //!< Current hover ui element.
    SceneUIElement *m_focusUIElement;  //!< Current focus ui element.

    //! Mapping of picking id to scene UI elements (can have multiple picking id to a same element)
    std::unordered_map<UInt32, SceneUIElement*> m_pickingToSceneUIElements;

    void processCommands();
    void postPicking(const Vector3f &position);
    Entity *findSelectable(Entity *entity) const;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENE_H

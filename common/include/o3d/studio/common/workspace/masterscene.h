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
        ACTION_STRETCH
    };

    enum SpeedModifier
    {
        SPEED_NORMAL = 0,
        SPEED_SLOW,
        SPEED_FAST
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


public /*slots*/:

    void pickingHit(Pickable* pickable, Vector3 pos);

private:

    Entity *m_parent;             //!< Parent entity (project, fragment)

    O3DCanvasContent *m_content;  //!< Attached widget
    o3d::Renderer *m_renderer;    //!< Attached renderer
    o3d::Scene *m_scene;          //!< Related o3d scene

    Point2i m_lockedPos;            //!< Infinite cursor
    ActionMode m_actionMode;        //!< Current action mode
    SpeedModifier m_speedModifier;  //!< Current action speed modifier

    UInt32 m_verticesCount[4];
    UInt32 m_trianglesCount[4];
    UInt32 m_linesCount[4];
    UInt32 m_pointCount[4];

    Hub *m_hoverHub;              //!< Current hub hovered by cursor

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

    void processCommands();
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENE_H

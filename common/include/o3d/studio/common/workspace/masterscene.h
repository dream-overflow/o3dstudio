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

namespace o3d {

class Scene;
class Camera;
class ViewPort;
class SceneDrawer;

namespace studio {
namespace common {

class O3DCanvasContent;
class QtRenderer;
class SceneCommand;

class Event;
class KeyEvent;
class MouseEvent;
class FocusEvent;

class O3S_API MasterScene : public BaseObject, public O3DDrawer
{
public:

    MasterScene(Entity *parent);
    virtual ~MasterScene();

    const Entity* parent() const;
    Entity* parent();

    const Project* project() const;
    Project* project();

    QtRenderer* renderer();
    O3DCanvasContent* widget();

    o3d::Scene* scene();
    const o3d::Scene* scene() const;

    O3DCanvasContent *content();
    const O3DCanvasContent *content() const;

    virtual void initialize(Bool debug = False);

    virtual void initializeDrawer() override;
    virtual void paintDrawer() override;
    virtual void updateDrawer() override;
    virtual void resizeDrawer(int w, int h) override;
    virtual void terminateDrawer() override;

    virtual void mousePressEvent(const MouseEvent &event);
    virtual void mouseReleaseEvent(const MouseEvent &event);
    virtual void mouseDoubleClickEvent(const MouseEvent &event);
    virtual void mouseMoveEvent(const MouseEvent &event);
    virtual void wheelEvent(const MouseEvent &event);

    virtual void keyPressEvent(const KeyEvent &event);
    virtual void keyReleaseEvent(const KeyEvent &event);

    virtual void focusInEvent(const Event &event);
    virtual void focusOutEvent(const Event &event);

    virtual void enterEvent(const Event &event);
    virtual void leaveEvent(const Event &event);

private:

    Entity *m_parent;             //!< Parent entity (project, fragment)

    O3DCanvasContent *m_content;  //!< Attached widget
    QtRenderer *m_renderer;       //!< Attached renderer
    o3d::Scene *m_scene;          //!< Related o3d scene

    Point2i m_localPos;
    Bool m_rotateCam;

    //! Main working camera, cannot be deleted
    o3d::SmartObject<o3d::Camera> m_camera;

    //! Main working viewport, cannot be deleted
    o3d::SmartObject<o3d::ViewPort> m_viewport;

    //! Main working drawer, cannot be deleted
    o3d::SmartObject<o3d::SceneDrawer> m_sceneDrawer;

    //! Ordered list of command to process during the moment where the context is set to current.
    //! In others words it is the pass of synchronization that will be executed here.
    std::list<SceneCommand*> m_commands;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENE_H

/**
 * @brief Workspace master scene rendering and management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_MASTERSCENE_H
#define _O3DS_COMMON_MASTERSCENE_H

#include <QtCore/QString>

#include "common/workspace/workspace.h"
#include "common/ui/qtrenderer.h"
#include "common/ui/canvas/o3ddrawer.h"

namespace o3d {
namespace studio {
namespace common {

class O3DCanvasContent;

class MasterScene : public O3DDrawer
{
public:

    MasterScene(Project *project);
    virtual ~MasterScene();

    const Project* project() const;
    Project* project();

    QtRenderer* renderer();
    O3DCanvasContent* widget();

    virtual void initialize();

    virtual void initializeDrawer() override;
    virtual void paintDrawer() override;
    virtual void updateDrawer() override;
    virtual void resizeDrawer(int w, int h) override;
    virtual void terminateDrawer() override;

private:

    Project *m_project;           //!< Owner project

    O3DCanvasContent *m_content;  //!< Attached widget
    QtRenderer *m_renderer;       //!< Attached renderer
    o3d::Scene *m_scene;          //!< Related o3d scene
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MASTERSCENE_H

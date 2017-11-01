/**
 * @brief Common OpenGL 4.5 canvas content with O3D specialisation
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-01
 * @details
 */

#ifndef _O3DS_COMMON_O3DCANVASCONTENT_H
#define _O3DS_COMMON_O3DCANVASCONTENT_H

#include "glcanvascontent.h"
#include "../qtrenderer.h"
#include "o3ddrawer.h"

namespace o3d {
namespace studio {
namespace common {

class O3DCanvasContent : public GLCanvasContent
{
public:

    explicit O3DCanvasContent(const QString &suffix, bool debug = false, QWidget *parent = nullptr);
    virtual ~O3DCanvasContent();

    virtual QString elementName() const override;

    void setRenderer(QtRenderer *renderer);

    const QtRenderer *renderer() const;
    QtRenderer *renderer();

    void setDrawer(O3DDrawer *drawer);

public slots:

    //void renderLater();
    // void renderNow();

protected:

    virtual void closeEvent(QCloseEvent*);

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:

    QString m_suffix;

    bool m_debug;
    QtRenderer *m_renderer;

    O3DDrawer *m_drawer;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_O3DCANVASCONTENT_H

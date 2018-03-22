/**
 * @brief Application main window transform toolbar.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-22
 * @details
 */

#ifndef _O3DS_MAIN_TRANSFORMTOOLBAR_H
#define _O3DS_MAIN_TRANSFORMTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "o3d/studio/common/ui/toolbar.h"
#include "o3d/studio/common/objectref.h"

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace main {

/**
 * @brief Application main window transform toolbar.
 */
class TransformToolBar : public BaseObject, public common::ToolBar
{
public:

    TransformToolBar(BaseObject *parent = nullptr);
    virtual ~TransformToolBar();

    virtual QToolBar* ui() override;
    virtual String elementName() const override;
    virtual Qt::ToolBarArea toolBarArea() const override;

private /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onProjectActivated(common::LightRef ref);
    void onProjectRemoved(common::LightRef ref);

private:

    class QtTransformToolBar *m_qtTransformToolBar;

    void setupUi();
};

class QtTransformToolBar : public QToolBar
{
    Q_OBJECT

public:

    QtTransformToolBar(QWidget *parent = nullptr);
    virtual ~QtTransformToolBar();

private slots:

    void onChangeTransformMode();

private:

    void setupUi();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_TRANSFORMTOOLBAR_H

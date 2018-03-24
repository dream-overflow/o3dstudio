/**
 * @brief Application transform toolbar.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-22
 * @details
 */

#ifndef _O3DS_COMMON_TRANSFORMTOOLBAR_H
#define _O3DS_COMMON_TRANSFORMTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "../toolbar.h"
#include "../../objectref.h"

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace common {

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

public /*singnals*/:

    Signal<Int32> onChangeTransformMode{this};
    Signal<Int32> onChangeOrientationMode{this};
    Signal<Int32> onChangePivotMode{this};

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

    QtTransformToolBar(TransformToolBar *model, QWidget *parent = nullptr);
    virtual ~QtTransformToolBar();

    void setActivity(Bool active);

private slots:

    void onChangeTransformMode();
    void onChangeOrientationMode(int vm);
    void onChangePivotMode(int pm);

private:

    void setupUi();

    TransformToolBar *m_model;

    class QComboBox *m_orientationMode;
    class QComboBox *m_pivotMode;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TRANSFORMTOOLBAR_H

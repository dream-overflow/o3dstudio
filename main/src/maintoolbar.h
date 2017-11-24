/**
 * @brief Application main window main toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_MAINTOOLBAR_H
#define _O3DS_MAIN_MAINTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "o3d/studio/common/ui/toolbar.h"
#include "o3d/studio/common/objectref.h"

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

class MainToolBar : public BaseObject, public common::ToolBar
{
public:

    MainToolBar(BaseObject *parent = nullptr);
    virtual ~MainToolBar();

    virtual QToolBar* ui() override;
    virtual String elementName() const override;
    virtual Qt::ToolBarArea toolBarArea() const override;

private /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onProjectActivated(LightRef ref);
    void onProjectRemoved(LightRef ref);

private:

    class QtMainToolBar *m_qtMainToolBar;

    void setupButtons();
    QIcon tintIcon(const QString &filename);

    void setupUi();
};

class QtMainToolBar : public QToolBar
{
    Q_OBJECT

public:

    QtMainToolBar(QWidget *parent = nullptr);
    virtual ~QtMainToolBar();

private slots:

    void onCreateFragment();
    void onCreateHub();

private:

    void setupButtons();
    QIcon tintIcon(const QString &filename);
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINTOOLBAR_H

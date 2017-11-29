/**
 * @brief Application main window main toolbar, contextual hub component toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_HUBCOMPONENTTOOLBAR_H
#define _O3DS_MAIN_HUBCOMPONENTTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "o3d/studio/common/ui/toolbar.h"
#include "o3d/studio/common/objectref.h"

#include <o3d/core/baseobject.h>
#include <o3d/core/vector2.h>

namespace o3d {
namespace studio {
namespace main {

/**
 * @brief Application main window main toolbar, contextual hub component toolbar.
 */
class HubComponentToolBar : public BaseObject // , public common::ToolBar
{
public:

    HubComponentToolBar(const Point2i &pos, BaseObject *parent = nullptr);
    virtual ~HubComponentToolBar();

//    virtual QToolBar* ui() override;
//    virtual String elementName() const override;
//    virtual Qt::ToolBarArea toolBarArea() const override;

    void show();

private /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onProjectActivated(common::LightRef ref);
    void onProjectRemoved(common::LightRef ref);

private:

    class QtHubComponentToolBar *m_qtHubComponentToolBar;
    Point2i m_pos;

    void setupUi();
};

class QtHubComponentToolBar : public QWidget
{
    Q_OBJECT

public:

    QtHubComponentToolBar(const QPoint &pos, QWidget *parent = nullptr);
    virtual ~QtHubComponentToolBar();

private slots:

    void onCreateHub();

private:

    void setupUi();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_HUBCOMPONENTTOOLBAR_H

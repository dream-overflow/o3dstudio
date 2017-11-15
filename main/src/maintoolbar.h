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

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

class MainToolBar : public QToolBar, public common::ToolBar
{
    Q_OBJECT

public:

    MainToolBar(QWidget *parent = nullptr);
    virtual ~MainToolBar();

    virtual QToolBar* ui() override;
    virtual QString elementName() const override;
    virtual Qt::ToolBarArea toolBarArea() const override;

signals:

protected:

private slots:

    void onChangeCurrentWorkspace(const QString &name);

    void onProjectActivated(const LightRef &ref);
    void onProjectRemoved(const LightRef &ref);

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

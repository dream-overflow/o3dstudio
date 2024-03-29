/**
 * @brief Application main window quick left fixed toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_QUICKTOOLBAR_H
#define _O3DS_MAIN_QUICKTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "o3d/studio/common/ui/toolbar.h"

namespace o3d {
namespace studio {
namespace main {

class QuickToolBar : public QToolBar, public common::ToolBar
{
    Q_OBJECT

public:

    QuickToolBar(QWidget *parent = nullptr);
    virtual ~QuickToolBar();

    virtual QToolBar* ui() override;
    virtual String elementName() const override;
    virtual Qt::ToolBarArea toolBarArea() const override;

signals:

    void showHome();

protected:

private slots:

    void onHomeBtn();

private:

    void setupButtons();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_QUICKTOOLBAR_H

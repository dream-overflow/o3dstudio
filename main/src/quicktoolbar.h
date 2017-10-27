/**
 * @brief Application main window quick left fixed toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_QUICKTOOLBAR_H
#define _O3DS_MAIN_QUICKTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "common/ui/toolbar.h"

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

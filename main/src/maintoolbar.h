/**
 * @brief Application main window main toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_MAINTOOLBAR_H
#define _O3DS_MAIN_MAINTOOLBAR_H

#include <QtWidgets/QToolBar>
#include "o3d/studio/common/ui/toolbar.h"

namespace o3d {
namespace studio {
namespace main {

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

private:

    void setupButtons();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINTOOLBAR_H

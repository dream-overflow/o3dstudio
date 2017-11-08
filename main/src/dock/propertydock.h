/**
 * @brief Application main property dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_MAIN_PROPERTYDOCK_H
#define _O3DS_MAIN_PROPERTYDOCK_H

class QTreeView;

#include <QtWidgets/QDockWidget>
#include "o3d/studio/common/ui/dock.h"

namespace o3d {
namespace studio {
namespace main {

class PropertyDock : public QDockWidget, public common::Dock
{
    Q_OBJECT

public:

    PropertyDock(QWidget *parent = nullptr);
    virtual ~PropertyDock();

    virtual QDockWidget* ui() override;
    virtual QString elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

signals:

protected:

public slots:

    void onSelectionChanged();

private:

    void setupUi();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PROPERTYDOCK_H

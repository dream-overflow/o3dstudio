/**
 * @brief Application main property dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_MAIN_PROPERTYDOCK_H
#define _O3DS_MAIN_PROPERTYDOCK_H

class QTreeView;

#include <QtWidgets/QDockWidget>
#include "o3d/studio/common/ui/dock.h"

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace main {

class PropertyDock : public BaseObject, public common::Dock
{
public:

    PropertyDock(BaseObject *parent = nullptr);
    virtual ~PropertyDock();

    virtual QDockWidget* ui() override;
    virtual String elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

public /*slots*/:

    void onSelectionChanged();

private:

    class QtPropertyDock *m_qtPropertyDock;

    void setupUi();
};

class QtPropertyDock : public QDockWidget
{
    Q_OBJECT

public:

    QtPropertyDock(QWidget *parent = nullptr);
    virtual ~QtPropertyDock();

signals:

protected:

public slots:

private:

    void setupUi();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PROPERTYDOCK_H

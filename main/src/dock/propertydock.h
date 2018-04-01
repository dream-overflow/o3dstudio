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
#include "o3d/studio/common/ui/panel.h"
#include "o3d/studio/common/objectref.h"

#include <o3d/core/baseobject.h>
#include <o3d/core/templatebitset.h>

#include <map>

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

    void onAttachPanel(String name, common::Panel* panel);
    void onDetachPanel(String name, common::Panel* panel);

    void onChangeCurrentWorkspace(const String &name);
    void onProjectEntityChanged(common::LightRef ref, BitSet64 changeFlags);

private:

    std::map<common::LightRef, common::Panel*> m_panels;   //!< Current panels

    class QtPropertyDock *m_qtPropertyDock;
};

class QtPropertyDock : public QDockWidget
{
    Q_OBJECT

public:

    QtPropertyDock(QWidget *parent = nullptr);
    virtual ~QtPropertyDock();

    class QLayout *container();

    void setupUi();

signals:

public slots:

private:

    class QWidget *m_container;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PROPERTYDOCK_H

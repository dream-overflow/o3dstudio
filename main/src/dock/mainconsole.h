/**
 * @brief Application main console dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-31
 * @details
 */

#ifndef _O3DS_MAIN_MAINCONSOLE_H
#define _O3DS_MAIN_MAINCONSOLE_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>

#include "o3d/studio/common/ui/dock.h"

#include <o3d/core/baseobject.h>

namespace o3d {
namespace studio {
namespace main {

class QtMainConsole;

class MainConsole : public BaseObject, public common::Dock
{
public:

    MainConsole(BaseObject *parent = nullptr);
    virtual ~MainConsole();

    virtual QDockWidget* ui() override;
    virtual String elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

public /*slots*/:

    void onMessage(UInt32 msgType, const String &message);

private:

    QtMainConsole *m_qtMainConsole;
};

class QtMainConsole : public QDockWidget
{
    Q_OBJECT

    friend class MainConsole;

public:

    QtMainConsole(QWidget *parent = nullptr);
    virtual ~QtMainConsole();

private:

    void setupUi();

    QListWidget *m_listWidget;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINCONSOLE_H

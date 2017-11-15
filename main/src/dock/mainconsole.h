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

namespace o3d {
namespace studio {
namespace main {

class MainConsole : public QDockWidget, public common::Dock
{
    Q_OBJECT

public:

    MainConsole(QWidget *parent = nullptr);
    virtual ~MainConsole();

    virtual QDockWidget* ui() override;
    virtual QString elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

signals:

protected:

public slots:

    void onMessage(QtMsgType msgType, const QString &message);

private:

    void setupUi();

    QListWidget *m_listWidget;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MAINCONSOLE_H

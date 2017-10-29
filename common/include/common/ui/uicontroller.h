/**
 * @brief Common application ui controller
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_UICONTROLLER_H
#define _O3DS_COMMON_UICONTROLLER_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "content.h"
#include "dock.h"
#include "toolbar.h"

namespace o3d {
namespace studio {
namespace common {

class UiController: public QObject
{
    Q_OBJECT

public:

    UiController();
    virtual ~UiController();

    bool addContent(Content *content);
    bool addDock(Dock *dock);
    bool addToolBar(ToolBar *toolBar);

    bool removeContent(Content *content);
    bool removeDock(Dock *dock);
    bool removeToolBar(ToolBar *toolBar);

signals:

    void attachContent(QString name, QWidget *content);
    void attachDock(QString name, QDockWidget *dock, Qt::DockWidgetArea area);
    void attachToolBar(QString name, QToolBar *toolbar, Qt::ToolBarArea area);

    void detachContent(QString name, QWidget *content);
    void detachDock(QString name, QDockWidget *dock);
    void detachToolBar(QString name, QToolBar *toolbar);

private:

    QList<Content*> m_contents;
    QList<Dock*> m_docks;
    QList<ToolBar*> m_toolBars;

    //! Non copyable
    UiController(const UiController&);

    //! Non assignable
    void operator=(const UiController&);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_UICONTROLLER_H

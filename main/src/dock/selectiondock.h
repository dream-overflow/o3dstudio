/**
 * @brief Application main selection dock
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-18
 * @details
 */

#ifndef _O3DS_MAIN_SELECTIONDOCK_H
#define _O3DS_MAIN_SELECTIONDOCK_H

class QListWidget;

#include <QtWidgets/QDockWidget>

#include <o3d/core/baseobject.h>

#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/workspace/projectitem.h"

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

/**
 * @brief The SelectionDock display the actual selection.
 */
class SelectionDock : public BaseObject, public common::Dock
{
public:

    SelectionDock(BaseObject *parent = nullptr);
    virtual ~SelectionDock();

    virtual QDockWidget* ui() override;
    virtual String elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

    /**
     * @brief Set the auto expand on select state.
     */
    void setAutoFollow(Bool s);
    Bool isAutoFollow() const;

public /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onActivateProject(LightRef ref);
    void onSelectionChanged();

private:

    class QtSelectionDock *m_qtSelectionDock;
    common::ProjectItem *m_lastSelected;

    Bool m_autoFollow;

    void setupUi();
};

class QtSelectionDock : public QDockWidget
{
    Q_OBJECT

    friend class SelectionDock;

public:

    QtSelectionDock(QWidget *parent = nullptr);
    virtual ~QtSelectionDock();

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void setModel(QAbstractItemModel *model);

public slots:

//    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
//    void onSelectionDetails(const QModelIndex &);
    void onSelectItem(const QModelIndex &index);

public:

    void setupUi();

    QListWidget *m_listWidget;
    common::ProjectItem *m_lastSelected;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_SELECTIONDOCK_H

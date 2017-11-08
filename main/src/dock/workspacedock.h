/**
 * @brief Application main workspace dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_MAIN_WORKSPACEDOCK_H
#define _O3DS_MAIN_WORKSPACEDOCK_H

class QTreeView;

#include <QtWidgets/QDockWidget>
#include "o3d/studio/common/ui/dock.h"

namespace o3d {
namespace studio {
namespace main {

class WorkspaceDock : public QDockWidget, public common::Dock
{
    Q_OBJECT

public:

    WorkspaceDock(QWidget *parent = nullptr);
    virtual ~WorkspaceDock();

    virtual QDockWidget* ui() override;
    virtual QString elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

    virtual void focusInEvent(QFocusEvent *event) override;

signals:

protected:

public slots:

    void onChangeCurrentWorkspace(const QString &name);

    void onAddProject(const QUuid &uuid);
    void onRemoveProject(const QUuid &uuid);
    void onActivateProject(const QUuid &uuid);

    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSelectionDetails(const QModelIndex &);
    void onSelectItem(const QModelIndex &index);

    // void onAddHub(const QUuid &uuid);
    // void onRemoveHub(const QUuid &uuid);

    void onSelectManagerChange();

private:

    void setupUi();

    QTreeView *m_treeView;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_WORKSPACEDOCK_H

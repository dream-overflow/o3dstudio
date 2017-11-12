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
#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/workspace/projectitem.h"

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

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

    void onAddProject(const LightRef &ref);
    void onRemoveProject(const LightRef &ref);
    void onActivateProject(const LightRef &ref);

    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSelectionDetails(const QModelIndex &);
    void onSelectItem(const QModelIndex &index);

    void onProjectHubAdded(const LightRef &ref);
    void onProjectHubRemoved(const LightRef &ref);

    void onProjectFragmentAdded(const LightRef &ref);
    void onProjectFragmentRemoved(const LightRef &ref);

    void onProjectAssetAdded(const LightRef &ref);
    void onProjectAssetRemoved(const LightRef &ref);

    void onSelectManagerChange();

private:

    void setupUi();

    QTreeView *m_treeView;

    common::ProjectItem *m_lastSelected;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_WORKSPACEDOCK_H

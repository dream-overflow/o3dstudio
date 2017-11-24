/**
 * @brief Application main workspace dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-06
 * @details
 */

#ifndef _O3DS_MAIN_WORKSPACEDOCK_H
#define _O3DS_MAIN_WORKSPACEDOCK_H

class QTreeView;

#include <QtWidgets/QDockWidget>

#include <o3d/core/baseobject.h>

#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/workspace/projectitem.h"

namespace o3d {
namespace studio {
namespace main {

using o3d::studio::common::LightRef;

class WorkspaceDock : public BaseObject, public common::Dock
{
public:

    WorkspaceDock(BaseObject *parent = nullptr);
    virtual ~WorkspaceDock();

    virtual QDockWidget* ui() override;
    virtual String elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

public /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onAddProject(const LightRef &ref);
    void onRemoveProject(const LightRef &ref);
    void onActivateProject(const LightRef &ref);

    void onProjectHubAdded(const LightRef &ref);
    void onProjectHubRemoved(const LightRef &ref);

    void onProjectFragmentAdded(const LightRef &ref);
    void onProjectFragmentRemoved(const LightRef &ref);

    void onProjectAssetAdded(const LightRef &ref);
    void onProjectAssetRemoved(const LightRef &ref);

    void onSelectManagerChange();

private:

    class QtWorkspaceDock *m_qtWorkspaceDock;
    common::ProjectItem *m_lastSelected;

    void setupUi();
};

class QtWorkspaceDock : public QDockWidget
{
    Q_OBJECT

    friend class WorkspaceDock;

public:

    QtWorkspaceDock(QWidget *parent = nullptr);
    virtual ~QtWorkspaceDock();

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

public slots:

    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSelectionDetails(const QModelIndex &);
    void onSelectItem(const QModelIndex &index);

public:

    void setupUi();

    QTreeView *m_treeView;
    common::ProjectItem *m_lastSelected;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_WORKSPACEDOCK_H

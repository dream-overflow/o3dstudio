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
#include <o3d/core/templatebitset.h>

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

    /**
     * @brief Set the auto expand on select state.
     */
    void setAutoExpand(Bool s);
    Bool isAutoExpand() const;

public /*slots*/:

    void onChangeCurrentWorkspace(const String &name);

    void onAddProject(LightRef ref);
    void onRemoveProject(LightRef ref);
    void onActivateProject(LightRef ref);

    void onProjectHubAdded(LightRef ref);
    void onProjectHubRemoved(LightRef ref);

    void onProjectFragmentAdded(LightRef ref);
    void onProjectFragmentRemoved(LightRef ref);

    void onProjectAssetAdded(LightRef ref);
    void onProjectAssetRemoved(LightRef ref);

    void onProjectEntityChanged(LightRef ref, BitSet64 changes);

    void onSelectManagerChange();

private:

    class QtWorkspaceDock *m_qtWorkspaceDock;
    common::ProjectItem *m_lastSelected;

    Bool m_autoExpand;   //!< Auto expand on selection : default True

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

    void setModel(QAbstractItemModel *model);

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

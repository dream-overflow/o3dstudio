/**
 * @brief Application main window transform toolbar.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-22
 * @details
 */

#include "transformtoolbar.h"
#include "../mainwindow.h"

#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtCore/QList>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"
#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"

#include "o3d/studio/common/ui/uiutils.h"

using namespace o3d::studio::main;


TransformToolBar::TransformToolBar(BaseObject *parent) :
    BaseObject(parent)
{
    setupUi();

    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &TransformToolBar::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());
}

TransformToolBar::~TransformToolBar()
{
    // delete m_qtHubComponentToolBar;
}

QToolBar *TransformToolBar::ui()
{
    return m_qtTransformToolBar;
}

o3d::String TransformToolBar::elementName() const
{
    return "o3s::main::toolbar::transform";
}

Qt::ToolBarArea TransformToolBar::toolBarArea() const
{
    return Qt::TopToolBarArea;
}

void TransformToolBar::onProjectActivated(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtTransformToolBar->actions();
        for (QAction *action : actionsList) {
            action->setEnabled(true);
        }
    }
}

void TransformToolBar::onProjectRemoved(common::LightRef ref)
{
    if (ref.isValid()) {
        QList<QAction*> actionsList = m_qtTransformToolBar->actions();
        for (QAction *action : actionsList) {
            action->setEnabled(false);
        }
    }
}

void TransformToolBar::setupUi()
{
    m_qtTransformToolBar = new QtTransformToolBar();
}

void TransformToolBar::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectActivated.connect(this, &TransformToolBar::onProjectActivated);
        workspace->onProjectRemoved.connect(this, &TransformToolBar::onProjectRemoved);
    }
}

QtTransformToolBar::QtTransformToolBar(QWidget *parent) :
    QToolBar(tr("Transform toolbar"), parent)
{
    setWindowFlags(windowFlags() | Qt::Popup);
    setWindowIcon(QIcon("://icons/3d_rotation_black.svg"));
    // setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);

    setupUi();

    // @todo Set a border style
    // setStyle();
}

QtTransformToolBar::~QtTransformToolBar()
{

}

void QtTransformToolBar::onChangeTransformMode()
{

}

void QtTransformToolBar::setupUi()
{
    QAction *translate = new QAction(common::UiUtils::tintIcon("://icons/open_with_black.svg"), tr("Translation"));
    connect(translate, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    translate->setEnabled(false);
    addAction(translate);

    QAction *rotate = new QAction(common::UiUtils::tintIcon("://icons/3d_rotation_black.svg"), tr("Rotation"));
    connect(rotate, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    rotate->setEnabled(false);
    addAction(rotate);

    QAction *scale = new QAction(common::UiUtils::tintIcon("://icons/transform_black.svg"), tr("Scale factor"));
    connect(scale, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    scale->setEnabled(false);
    addAction(scale);

    QAction *skew = new QAction(common::UiUtils::tintIcon("://icons/details_black.svg"), tr("Skew"));
    connect(skew, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    skew->setEnabled(false);
    addAction(skew);

    QComboBox *viewMode = new QComboBox();
    addWidget(viewMode);

    QComboBox *pivotMode = new QComboBox();
    addWidget(pivotMode);
}

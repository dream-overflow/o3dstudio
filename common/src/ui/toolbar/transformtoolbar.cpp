/**
 * @brief Application transform toolbar.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-22
 * @details
 */

#include <QtWidgets/QComboBox>

#include "o3d/studio/common/ui/toolbar/transformtoolbar.h"

#include <QtWidgets/QToolButton>
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

using namespace o3d::studio::common;


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
        m_qtTransformToolBar->setActivity(true);
    }
}

void TransformToolBar::onProjectRemoved(common::LightRef ref)
{
    if (ref.isValid()) {
        m_qtTransformToolBar->setActivity(false);
    }
}

void TransformToolBar::setupUi()
{
    m_qtTransformToolBar = new QtTransformToolBar(this);
}

void TransformToolBar::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectActivated.connect(this, &TransformToolBar::onProjectActivated);
        workspace->onProjectRemoved.connect(this, &TransformToolBar::onProjectRemoved);
    }
}

QtTransformToolBar::QtTransformToolBar(TransformToolBar *model, QWidget *parent) :
    QToolBar(tr("Transform toolbar"), parent),
    m_model(model)
{
    setWindowFlags(windowFlags() | Qt::Popup);
    setWindowIcon(QIcon("://icons/3d_rotation_black.svg"));
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);

    setupUi();
}

QtTransformToolBar::~QtTransformToolBar()
{

}

void QtTransformToolBar::setActivity(o3d::Bool active)
{
    QList<QAction*> actionsList = actions();
    for (QAction *action : actionsList) {
        action->setEnabled(active);
    }

    if (active) {
        actionsList.first()->setChecked(true);
    }

    m_orientationMode->setEnabled(active);
    m_pivotMode->setEnabled(active);
}

void QtTransformToolBar::onChangeTransformMode()
{
    m_model->onChangeTransformMode(sender()->property("tranformMode").toInt(0));
}

void QtTransformToolBar::onChangeOrientationMode(int vm)
{
    m_model->onChangeOrientationMode(vm);
}

void QtTransformToolBar::onChangePivotMode(int pm)
{
    m_model->onChangePivotMode(pm);
}

void QtTransformToolBar::setupUi()
{
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    QAction *translate = new QAction(common::UiUtils::tintIcon("://icons/open_with_black.svg"), tr("Translation"), actionGroup);
    connect(translate, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    translate->setProperty("tranformMode", QVariant(0));
    translate->setEnabled(false);
    translate->setCheckable(true);
    translate->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_T));
    addAction(translate);

    QAction *rotate = new QAction(common::UiUtils::tintIcon("://icons/3d_rotation_black.svg"), tr("Rotation"), actionGroup);
    connect(rotate, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    rotate->setProperty("tranformMode", QVariant(1));
    rotate->setEnabled(false);
    rotate->setCheckable(true);
    rotate->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_R));
    addAction(rotate);

    QAction *scale = new QAction(common::UiUtils::tintIcon("://icons/transform_black.svg"), tr("Scale factor"), actionGroup);
    connect(scale, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    scale->setProperty("tranformMode", QVariant(2));
    scale->setEnabled(false);
    scale->setCheckable(true);
    scale->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_S));
    addAction(scale);

    QAction *skew = new QAction(common::UiUtils::tintIcon("://icons/details_black.svg"), tr("Skew"), actionGroup);
    connect(skew, SIGNAL(triggered(bool)), SLOT(onChangeTransformMode()));
    skew->setProperty("tranformMode", QVariant(3));
    skew->setEnabled(false);
    skew->setCheckable(true);
    skew->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_K));
    addAction(skew);

    m_orientationMode = new QComboBox();
    m_orientationMode->setEnabled(false);

    m_orientationMode->blockSignals(true);
    m_orientationMode->addItem(common::UiUtils::tintIcon("://icons/fullscreen_exit_black.svg"), tr("Local"), QVariant(1));
    m_orientationMode->addItem(common::UiUtils::tintIcon("://icons/fullscreen_black.svg"), tr("Global"), QVariant(0));
    m_orientationMode->addItem(common::UiUtils::tintIcon("://icons/image_black.svg"), tr("View"), QVariant(2));
    m_orientationMode->blockSignals(false);

    addWidget(m_orientationMode);
    connect(m_orientationMode, SIGNAL(activated(int)), SLOT(onChangeOrientationMode(int)));

    m_pivotMode = new QComboBox();
    m_pivotMode->setEnabled(false);

    m_pivotMode->blockSignals(true);
    m_pivotMode->addItem(common::UiUtils::tintIcon("://icons/center_focus_strong_black.svg"), tr("Active"), QVariant(0));
    m_pivotMode->addItem(common::UiUtils::tintIcon("://icons/center_focus_weak_black.svg"), tr("Invidual"), QVariant(1));
    m_pivotMode->addItem(common::UiUtils::tintIcon("://icons/clear_all_black.svg"), tr("Median"), QVariant(2));
    m_pivotMode->addItem(common::UiUtils::tintIcon("://icons/close_black.svg"), tr("User"), QVariant(3));
    m_pivotMode->blockSignals(false);

    addWidget(m_pivotMode);
    connect(m_pivotMode, SIGNAL(activated(int)), SLOT(onChangePivotMode(int)));
}

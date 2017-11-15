/**
 * @brief Application main property dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-06
 * @details
 */

#include "propertydock.h"
#include "../mainwindow.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/projectmodel.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::main;


PropertyDock::PropertyDock(QWidget *parent) :
    QDockWidget(tr("Property"), parent),
    common::Dock()
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();

    // selection manager
    connect(&common::Application::instance()->selection(), SIGNAL(selectionChanged()), SLOT(onSelectionChanged()));
}

PropertyDock::~PropertyDock()
{

}

QDockWidget *PropertyDock::ui()
{
    return this;
}

QString PropertyDock::elementName() const
{
    return("o3s::main::property");
}

Qt::DockWidgetArea PropertyDock::dockWidgetArea() const
{
    return Qt::RightDockWidgetArea;
}

void PropertyDock::onSelectionChanged()
{
    const QSet<common::SelectionItem *> previousSelection =
            common::Application::instance()->selection().filterPreviousByBaseType(common::TypeRef::project());
    const QSet<common::SelectionItem *> currentSelection =
            common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::project());

    common::SelectionItem *selectionItem = nullptr;
    foreach (selectionItem, previousSelection) {

    }

    selectionItem = nullptr;
    foreach (selectionItem, currentSelection) {
    }
}

void PropertyDock::setupUi()
{
    setWindowIcon(QIcon::fromTheme("document-properties"));
}

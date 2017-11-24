/**
 * @brief Application main property dock
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
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


PropertyDock::PropertyDock(BaseObject *parent) :
    BaseObject(parent),
    common::Dock()
{
    setupUi();

    // selection manager
    common::Application::instance()->selection().selectionChanged.connect(this, &PropertyDock::onSelectionChanged);
}

PropertyDock::~PropertyDock()
{
    // deletePtr(m_qtPropertyDock);
}

QDockWidget *PropertyDock::ui()
{
    return m_qtPropertyDock;
}

o3d::String PropertyDock::elementName() const
{
    return "o3s::main::property";
}

Qt::DockWidgetArea PropertyDock::dockWidgetArea() const
{
    return Qt::RightDockWidgetArea;
}

void PropertyDock::onSelectionChanged()
{
    const std::set<common::SelectionItem *> previousSelection =
            common::Application::instance()->selection().filterPreviousByBaseType(common::TypeRef::project());
    const std::set<common::SelectionItem *> currentSelection =
            common::Application::instance()->selection().filterCurrentByBaseType(common::TypeRef::project());

    for (common::SelectionItem *selectionItem : previousSelection) {

    }

    for (common::SelectionItem *selectionItem : currentSelection) {

    }
}

void PropertyDock::setupUi()
{
    m_qtPropertyDock = new QtPropertyDock();
}


QtPropertyDock::QtPropertyDock(QWidget *parent) :
    QDockWidget(tr("Property"), parent)
{
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupUi();
}

QtPropertyDock::~QtPropertyDock()
{

}

void QtPropertyDock::setupUi()
{
    setWindowIcon(QIcon::fromTheme("document-properties"));
}

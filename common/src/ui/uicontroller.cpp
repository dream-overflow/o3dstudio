/**
 * @brief Common application ui controller
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>

#include "common/ui/uicontroller.h"

using namespace o3d::studio::common;


UiController::UiController() :
    QObject()
{

}

UiController::~UiController()
{

}

bool UiController::addContent(Content *content)
{
    if (content == nullptr) {
        return false;
    }

    if (m_contents.indexOf(content) >= 0) {
        return false;
    }

    m_contents.append(content);
    emit attachContent(content->elementName(), content->ui());

    return true;
}

bool UiController::addDock(Dock *dock)
{
    if (dock == nullptr) {
        return false;
    }

    if (m_docks.indexOf(dock) >= 0) {
        return false;
    }

    m_docks.append(dock);
    emit attachDock(dock->elementName(), dock->ui(), dock->dockWidgetArea());

    return true;
}

bool UiController::addToolBar(ToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return false;
    }

    if (m_toolBars.indexOf(toolBar) >= 0) {
        return false;
    }

    m_toolBars.append(toolBar);
    emit attachToolBar(toolBar->elementName(), toolBar->ui(), toolBar->toolBarArea());

    return true;
}

bool UiController::removeContent(Content *content)
{
    if (content == nullptr) {
        return false;
    }

    int index = -1;
    if ((index = m_contents.indexOf(content)) >= 0) {
        m_contents.removeAt(index);
        emit detachContent(content->elementName(), content->ui());

        return true;
    }

    return false;
}

bool UiController::removeDock(Dock *dock)
{
    if (dock == nullptr) {
        return false;
    }

    int index = -1;
    if ((index = m_docks.indexOf(dock)) >= 0) {
        m_docks.removeAt(index);
        emit detachDock(dock->elementName(), dock->ui());

        return true;
    }

    return false;
}

bool UiController::removeToolBar(ToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return false;
    }

    int index = -1;
    if ((index = m_toolBars.indexOf(toolBar)) >= 0) {
        m_toolBars.removeAt(index);
        emit detachToolBar(toolBar->elementName(), toolBar->ui());

        return true;
    }

    return false;
}

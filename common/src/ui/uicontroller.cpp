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
    QObject(),
    m_activeContent(nullptr)
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

        if (m_activeContent == content) {
            m_activeContent = nullptr;
        }

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

bool UiController::setActiveContent(Content *content, bool showHide)
{
    if (content == nullptr) {
        return false;
    }

    int index = -1;
    if ((index = m_contents.indexOf(content)) >= 0) {
        if (showHide) {
            if (m_activeContent == content) {
                return true;
            } else {
                m_activeContent = content;
                emit showContent(content->elementName(), content->ui(), true);
            }
        } else {
            if (m_activeContent != content) {
                return true;
            } else {
                m_activeContent = nullptr;
                emit showContent(content->elementName(), content->ui(), false);

                // uses the last in the list
                if (!m_contents.isEmpty()) {
                    m_activeContent = m_contents.back();

                    emit showContent(m_activeContent->elementName(), m_activeContent->ui(), true);
                }
            }
        }

        return true;
    }

    return false;
}

Content *UiController::content(const QString &name)
{
    Content *content = nullptr;
    foreach(content, m_contents) {
        if (content->elementName() == name) {
            return content;
        }
    }

    return nullptr;
}

const Content *UiController::content(const QString &name) const
{
    const Content *content = nullptr;
    foreach(content, m_contents) {
        if (content->elementName() == name) {
            return content;
        }
    }

    return nullptr;
}

Dock *UiController::dock(const QString &name)
{
    Dock *dock = nullptr;
    foreach(dock, m_docks) {
        if (dock->elementName() == name) {
            return dock;
        }
    }

    return nullptr;
}

const Dock *UiController::dock(const QString &name) const
{
    const Dock *dock = nullptr;
    foreach(dock, m_docks) {
        if (dock->elementName() == name) {
            return dock;
        }
    }

    return nullptr;
}

ToolBar *UiController::toolBar(const QString &name)
{
    ToolBar *toolBar = nullptr;
    foreach(toolBar, m_toolBars) {
        if (toolBar->elementName() == name) {
            return toolBar;
        }
    }

    return nullptr;
}

const ToolBar *UiController::toolBar(const QString &name) const
{
    const ToolBar *toolBar = nullptr;
    foreach(toolBar, m_toolBars) {
        if (toolBar->elementName() == name) {
            return toolBar;
        }
    }

    return nullptr;
}

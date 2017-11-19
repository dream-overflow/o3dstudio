/**
 * @brief Common application ui controller
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include "o3d/studio/common/ui/uicontroller.h"

#include <algorithm>

using namespace o3d::studio::common;

Element::~Element()
{
}

UiController::UiController() :
    BaseObject(),
    m_activeContent(nullptr)
{

}

UiController::~UiController()
{
    // docks
    for (auto it = m_docks.begin(); it != m_docks.end(); ++it) {
        if ((*it)->ui()) {
            (*it)->ui()->setParent(nullptr);
            delete (*it)->ui();
        }

        if (reinterpret_cast<void*>(*it) != reinterpret_cast<void*>((*it)->ui())) {
            delete *it;
        }
    }

    // toolbars
    for (auto it = m_toolBars.begin(); it != m_toolBars.end(); ++it) {
        if ((*it)->ui()) {
            (*it)->ui()->setParent(nullptr);
            delete (*it)->ui();
        }

        if (reinterpret_cast<void*>(*it) != reinterpret_cast<void*>((*it)->ui())) {
            delete *it;
        }
    }

    // contents
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it) {
        if ((*it)->ui()) {
            (*it)->ui()->setParent(nullptr);
            delete (*it)->ui();
        }

        if (reinterpret_cast<void*>(*it) != reinterpret_cast<void*>((*it)->ui())) {
            delete *it;
        }
    }
}

o3d::Bool UiController::addContent(Content *content)
{
    if (content == nullptr) {
        return False;
    }

    auto it = std::find(m_contents.begin(), m_contents.end(), content);
    if (it != m_contents.end()) {
        return False;
    }

    if (content->ui()) {
        content->ui()->setProperty("name", toQString(content->elementName()));
    }

    m_contents.push_back(content);

    // emit
    attachContent(content->elementName(), content);

    return True;
}

o3d::Bool UiController::addDock(Dock *dock)
{
    if (dock == nullptr) {
        return False;
    }

    auto it = std::find(m_docks.begin(), m_docks.end(), dock);
    if (it != m_docks.end()) {
        return False;
    }

    if (dock->ui()) {
        dock->ui()->setProperty("name", toQString(dock->elementName()));
    }

    m_docks.push_back(dock);
    attachDock(dock->elementName(), dock);

    return True;
}

o3d::Bool UiController::addToolBar(ToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return False;
    }

    auto it = std::find(m_toolBars.begin(), m_toolBars.end(), toolBar);
    if (it != m_toolBars.end()) {
        return False;
    }

    if (toolBar->ui()) {
        toolBar->ui()->setProperty("name", toQString(toolBar->elementName()));
    }

    m_toolBars.push_back(toolBar);
    attachToolBar(toolBar->elementName(), toolBar);

    return True;
}

o3d::Bool UiController::removeContent(Content *content)
{
    if (content == nullptr) {
        return False;
    }

    auto it = std::find(m_contents.begin(), m_contents.end(), content);
    if (it != m_contents.end()) {
        m_contents.erase(it);

        if (m_activeContent == content) {
            m_activeContent = nullptr;
        }

        detachContent(content->elementName(), content);

        if (content->ui()) {
            content->ui()->setProperty("name", "");
        }

        content->ui()->setParent(nullptr);
        return True;
    }

    return False;
}

o3d::Bool UiController::removeContent(const String &name)
{
    Content *lcontent = content(name);
    return removeContent(lcontent);
}

o3d::Bool UiController::removeDock(Dock *dock)
{
    if (dock == nullptr) {
        return False;
    }

    auto it = std::find(m_docks.begin(), m_docks.end(), dock);
    if (it != m_docks.end()) {
        m_docks.erase(it);

        detachDock(dock->elementName(), dock);

        if (dock->ui()) {
            dock->ui()->setProperty("name", "");
        }

        dock->ui()->setParent(nullptr);
        return True;
    }

    return False;
}

o3d::Bool UiController::removeDock(const String &name)
{
    Dock *ldock = dock(name);
    return removeDock(ldock);
}

o3d::Bool UiController::removeToolBar(ToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return False;
    }

    auto it = std::find(m_toolBars.begin(), m_toolBars.end(), toolBar);
    if (it != m_toolBars.end()) {
        m_toolBars.erase(it);

        detachToolBar(toolBar->elementName(), toolBar);

        if (toolBar->ui()) {
            toolBar->ui()->setProperty("name", "");
        }

        toolBar->ui()->setParent(nullptr);
        return True;
    }

    return False;
}

o3d::Bool UiController::removeToolBar(const String &name)
{
    ToolBar *ltoolBar = toolBar(name);
    return removeToolBar(ltoolBar);
}

o3d::Bool UiController::setActiveContent(Content *content, o3d::Bool showHide)
{
    if (content == nullptr) {
        return False;
    }

    auto it = std::find(m_contents.begin(), m_contents.end(), content);
    if (it != m_contents.end()) {
        if (showHide) {
            if (m_activeContent == content) {
                return True;
            } else {
                m_activeContent = content;
                showContent(content->elementName(), content, true);
            }
        } else {
            if (m_activeContent != content) {
                return True;
            } else {
                m_activeContent = nullptr;
                showContent(content->elementName(), content, false);

                // uses the last in the list
                if (!m_contents.empty()) {
                    m_activeContent = m_contents.back();

                    showContent(m_activeContent->elementName(), m_activeContent, true);
                }
            }
        }

        return True;
    }

    return False;
}

Content *UiController::activeContent()
{
    return m_activeContent;
}

const Content *UiController::activeContent() const
{
    return m_activeContent;
}

Content *UiController::content(const String &name)
{
    for (Content *content : m_contents) {
        if (content->elementName() == name) {
            return content;
        }
    }

    return nullptr;
}

const Content *UiController::content(const String &name) const
{
    for (const Content *content : m_contents) {
        if (content->elementName() == name) {
            return content;
        }
    }

    return nullptr;
}

Dock *UiController::dock(const String &name)
{
    for (Dock *dock: m_docks) {
        if (dock->elementName() == name) {
            return dock;
        }
    }

    return nullptr;
}

const Dock *UiController::dock(const String &name) const
{
    for (const Dock *dock: m_docks) {
        if (dock->elementName() == name) {
            return dock;
        }
    }

    return nullptr;
}

ToolBar *UiController::toolBar(const String &name)
{
    for (ToolBar *toolBar: m_toolBars) {
        if (toolBar->elementName() == name) {
            return toolBar;
        }
    }

    return nullptr;
}

const ToolBar *UiController::toolBar(const String &name) const
{
    for (const ToolBar *toolBar: m_toolBars) {
        if (toolBar->elementName() == name) {
            return toolBar;
        }
    }

    return nullptr;
}

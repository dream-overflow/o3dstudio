/**
 * @brief Common application ui controller
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_UICONTROLLER_H
#define _O3DS_COMMON_UICONTROLLER_H

#include <o3d/core/baseobject.h>

#include "content.h"
#include "dock.h"
#include "toolbar.h"
#include "panel.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API UiController: public BaseObject
{
public:

    UiController();
    virtual ~UiController();

    /**
     * @brief A content is added as central widget, in a stacked area but only one
     * is active at time.
     * @param content Valid content with a ui to add to the central widget.
     * @return True if success.
     */
    Bool addContent(Content *content);

    /**
     * @brief A dock is a side panel, stackable, floatable, movable.
     * Many docks are actives at the same time.
     * @param dock Valid dock with a ui to add to a prefered area.
     * @return True if success.
     */
    Bool addDock(Dock *dock);

    /**
     * @brief A tool-bar is a pre-side widget, with a serialized organisation.
     * Many tool-bars are active at the same time.
     * @param toolBar Valid tool-bar with a ui to add to a prefered area.
     * @return True if success.
     */
    Bool addToolBar(ToolBar *toolBar);

    /**
     * @brief Add a panel to a widget that support its target type.
     * Panels are different from contents and dock, as they can be setup as modal,
     * into a master scene, or into a dock.
     * @note A panel is by nature more volatile than the 3 others elements,
     * because it is mostly linked to a selection, or to a temporary object.
     * @param panel Valid panel to setup.
     * @return True if success.
     */
    Bool addPanel(Panel *panel);

    Bool removeContent(Content *content);
    Bool removeDock(Dock *dock);
    Bool removeToolBar(ToolBar *toolBar);
    Bool removePanel(Panel *panel);

    Bool removeContent(const String &name);
    Bool removeDock(const String &name);
    Bool removeToolBar(const String &name);   
    Bool removePanel(const String &name);

    /**
     * @brief Set the active content. The unique one at time.
     * @param content Valid content to activate or disable.
     * @param showHide Show state, if False take the last as new current.
     * @return True if content state changed.
     */
    Bool setActiveContent(Content *content, Bool showHide);

    Content *activeContent();
    const Content *activeContent() const;

    Content* content(const String &name);
    const Content* content(const String &name) const;

    Dock* dock(const String &name);
    const Dock* dock(const String &name) const;

    ToolBar* toolBar(const String &name);
    const ToolBar* toolBar(const String &name) const;

    Panel* panel(const String &name);
    const Panel* panel(const String &name) const;

    UInt32 numContents() const;
    UInt32 numToolBars() const;
    UInt32 numDocks() const;
    UInt32 numPanels() const;

    Content* previousContent();
    Content* nextContent();

public /*signals*/:

    Signal<String /*name*/, Content* /*content*/> attachContent{this};
    Signal<String /*name*/, Dock* /*dock*/> attachDock{this};
    Signal<String /*name*/, ToolBar* /*toolBar*/> attachToolBar{this};
    Signal<String /*name*/, Panel* /*panel*/> attachPanel{this};

    Signal<String /*name*/, Content* /*content*/> detachContent{this};
    Signal<String /*name*/, Dock* /*dock*/> detachDock{this};
    Signal<String /*name*/, ToolBar* /*toolBar*/> detachToolBar{this};
    Signal<String /*name*/, Panel* /*panel*/> detachPanel{this};

    Signal<String /*name*/, Content* /*content*/, Bool /*showHide*/> showContent{this};

private:

    Content* m_activeContent;

    std::list<Content*> m_contents;
    std::list<Dock*> m_docks;
    std::list<ToolBar*> m_toolBars;
    std::list<Panel*> m_panels;

    //! Non copyable
    UiController(const UiController&);

    //! Non assignable
    void operator=(const UiController&);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_UICONTROLLER_H

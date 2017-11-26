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

namespace o3d {
namespace studio {
namespace common {

class O3S_API UiController: public BaseObject
{
public:

    UiController();
    virtual ~UiController();

    Bool addContent(Content *content);
    Bool addDock(Dock *dock);
    Bool addToolBar(ToolBar *toolBar);

    Bool removeContent(Content *content);
    Bool removeDock(Dock *dock);
    Bool removeToolBar(ToolBar *toolBar);

    Bool removeContent(const String &name);
    Bool removeDock(const String &name);
    Bool removeToolBar(const String &name);

    Bool setActiveContent(Content *content, Bool showHide);
    Content *activeContent();
    const Content *activeContent() const;

    Content* content(const String &name);
    const Content* content(const String &name) const;

    Dock* dock(const String &name);
    const Dock* dock(const String &name) const;

    ToolBar* toolBar(const String &name);
    const ToolBar* toolBar(const String &name) const;

    Int32 numContents() const;
    Int32 numToolBars() const;
    Int32 numDocks() const;

    Content* previousContent();
    Content* nextContent();

public /*signals*/:

    Signal<String /*name*/, Content* /*content*/> attachContent{this};
    Signal<String /*name*/, Dock* /*content*/> attachDock{this};
    Signal<String /*name*/, ToolBar* /*content*/> attachToolBar{this};

    Signal<String /*name*/, Content* /*content*/> detachContent{this};
    Signal<String /*name*/, Dock* /*content*/> detachDock{this};
    Signal<String /*name*/, ToolBar* /*content*/> detachToolBar{this};

    Signal<String /*name*/, Content* /*content*/, Bool /*showHide*/> showContent{this};

private:

    Content* m_activeContent;

    std::list<Content*> m_contents;
    std::list<Dock*> m_docks;
    std::list<ToolBar*> m_toolBars;

    //! Non copyable
    UiController(const UiController&);

    //! Non assignable
    void operator=(const UiController&);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_UICONTROLLER_H

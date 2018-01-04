/**
 * @brief Common UI panel element and toolset
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-08
 * @details
 */

#ifndef _O3DS_COMMON_PANEL_H
#define _O3DS_COMMON_PANEL_H

#include <QtWidgets/QWidget>
#include "../element.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API Panel : public Element
{
public:

    enum PanelType
    {
        PANEL_PROPERTY,    //!< Going to property dock
        PANEL_POPUP,       //!< Into a modal window
        PANEL_WIDGET       //!< Into a pure o3d widget
    };

    /**
     * @brief Returns the UI object.
     */
    virtual QWidget* ui() = 0;

    /**
     * @brief Type of panel.
     */
    virtual PanelType panelType() const = 0;

    /**
     * @brief Commit the changes done on this panel to its target element (hub...).
     */
    virtual void commit() = 0;

    /**
     * @brief Update the changes from the target element (hub...).
     */
    virtual void update() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PANEL_H

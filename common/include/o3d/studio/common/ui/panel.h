/**
 * @brief Common UI panrel element
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

    virtual QWidget* ui() = 0;

    virtual PanelType panelType() const = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PANEL_H

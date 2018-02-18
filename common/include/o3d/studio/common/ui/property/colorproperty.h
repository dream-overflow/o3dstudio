/**
 * @brief Panel property with 3 or 4 components color RGB(A).
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_COLORPROPERTY_H
#define _O3DS_COMMON_COLORPROPERTY_H

#include "panelproperty.h"
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Color RGB(A) property.
 * @todo color picker and which format (float, hex, int...)
 */
class O3S_API ColorProperty : public PanelProperty
{
public:

    enum ColorFormat
    {
        COLOR_RGB = 0,
        COLOR_RGBA = 1
    };

    //! Constructor
    ColorProperty(Panel *panel, const String &name, const String &label, ColorFormat format);

    //! Destructor
    virtual ~ColorProperty();

    //! Get the resulting widget.
    QWidget* ui();

    ColorFormat colorFormat() const;

    o3d::Color value() const;

public /*signals*/:

    Signal<Color> onValueChanged{this};

private:

    ColorFormat m_format;
    class QDoubleSpinBox *m_v[4];
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COLORPROPERTY_H

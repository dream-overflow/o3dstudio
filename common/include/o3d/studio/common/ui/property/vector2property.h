/**
 * @brief Panel property with 2 components vector.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_VECTOR2PROPERTY_H
#define _O3DS_COMMON_VECTOR2PROPERTY_H

#include "panelproperty.h"
#include <o3d/core/vector2.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Vector of 2 float property.
 */
class O3S_API Vector2Property : public PanelProperty
{
public:

    //! Constructor
    Vector2Property(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~Vector2Property();

    //! Get the resulting widget.
    QWidget* ui();

    void setMinMax(Float min, Float max);

    o3d::Vector2f value() const;
    void setValue(const Vector2f &v);

public /*signals*/:

    Signal<o3d::Vector2f> onValueChanged{this};

private:

    class QDoubleSpinBox *m_v[2];
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VECTOR2PROPERTY_H

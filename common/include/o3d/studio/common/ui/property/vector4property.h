/**
 * @brief Panel property with 4 components vector.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_VECTOR4PROPERTY_H
#define _O3DS_COMMON_VECTOR4PROPERTY_H

#include "panelproperty.h"
#include <o3d/core/vector4.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Vector of 4 float property.
 */
class O3S_API Vector4Property : public PanelProperty
{
public:

    //! Constructor
    Vector4Property(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~Vector4Property();

    //! Get the resulting widget.
    QWidget* ui();

    void setMinMax(Float min, Float max);

    o3d::Vector4f value() const;
    void setValue(const o3d::Vector4f &v);

public /*signals*/:

    Signal<o3d::Vector4f> onValueChanged{this};

private:

    class QDoubleSpinBox *m_v[4];
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VECTOR3PROPERTY_H

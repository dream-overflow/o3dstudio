/**
 * @brief Panel property with 3 components vector.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-21
 * @details
 */

#ifndef _O3DS_COMMON_VECTOR3PROPERTY_H
#define _O3DS_COMMON_VECTOR3PROPERTY_H

#include "panelproperty.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Panel property abstract model.
 */
class O3S_API Vector3Property : public PanelProperty
{
public:

    //! Constructor
    Vector3Property(Panel *panel, const String &label);

    //! Destructor
    virtual ~Vector3Property();

    //! Get the resulting widget.
    QWidget* ui();

public /*signals*/:

    Signal<Float, Float, Float> onValueChanged{this};
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VECTOR3PROPERTY_H

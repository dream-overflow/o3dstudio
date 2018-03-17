/**
 * @brief Panel property with 3 components a b and c for quadratic equation.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-17
 * @details
 */

#ifndef _O3DS_COMMON_QUADRATICPROPERTY_H
#define _O3DS_COMMON_QUADRATICPROPERTY_H

#include "panelproperty.h"
#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Panel property with 3 components a b and c for quadratic equation.
 */
class O3S_API QuadraticProperty : public PanelProperty
{
public:

    //! Constructor
    QuadraticProperty(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~QuadraticProperty();

    //! Get the resulting widget.
    QWidget* ui();

    void setMinMax(Float min, Float max);

    /**
     * @brief Format is constant, linear, quadratic (c,b,a)
     */
    o3d::Vector3f value() const;

    /**
     * @brief Format is constant, linear, quadratic (c,b,a)
     */
    void setValue(const o3d::Vector3f &v);

public /*signals*/:

    Signal<o3d::Vector3f> onValueChanged{this};

private:

    class QDoubleSpinBox *m_v[3];
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_QUADRATICPROPERTY_H

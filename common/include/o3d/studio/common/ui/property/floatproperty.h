/**
 * @brief Panel property with a single float.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_FLOATPROPERTY_H
#define _O3DS_COMMON_FLOATPROPERTY_H

#include "panelproperty.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Single float property.
 */
class O3S_API FloatProperty : public PanelProperty
{
public:

    //! Constructor
    FloatProperty(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~FloatProperty();

    //! Get the resulting widget.
    QWidget* ui();

    void setMinMax(Float min, Float max);
    void setPrecision(Int32 dec);

    o3d::Float value() const;
    void setValue(Float v);

    /**
     * @brief Setup a forbidden sub-range of value in exclusive (strict > and <).
     * @note min must be lesser than max.
     * @note If min is equal to max the range is empty, same as unset method.
     */
    void setForbiddenExclusiveRange(Float minExcl, Float maxExcl);
    void unsetForbiddenExclusiveRange();

public /*signals*/:

    Signal<Float> onValueChanged{this};

private:

    class QDoubleSpinBox *m_x;

    Float m_minExl;
    Float m_maxExl;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_FLOATPROPERTY_H

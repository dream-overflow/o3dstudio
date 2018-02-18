/**
 * @brief Panel property with a single boolean.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_BOOLPROPERTY_H
#define _O3DS_COMMON_BOOLPROPERTY_H

#include "panelproperty.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Single boolean property.
 */
class O3S_API BoolProperty : public PanelProperty
{
public:

    //! Constructor
    BoolProperty(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~BoolProperty();

    //! Get the resulting widget.
    QWidget* ui();

    o3d::Bool value() const;

public /*signals*/:

    Signal<Bool> onValueChanged{this};

private:

    class QCheckBox *m_b;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_BOOLPROPERTY_H

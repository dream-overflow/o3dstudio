/**
 * @brief Panel property with a drop down list of predefined values.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-03-15
 * @details
 */

#ifndef _O3DS_COMMON_DROPDOWNPROPERTY_H
#define _O3DS_COMMON_DROPDOWNPROPERTY_H

#include "panelproperty.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Property with a drop down list of predefined values.
 */
class O3S_API DropDownProperty : public PanelProperty
{
public:

    //! Constructor
    DropDownProperty(Panel *panel, const String &name, const String &label);

    //! Destructor
    virtual ~DropDownProperty();

    //! Get the resulting widget.
    QWidget* ui();

    void addValue(const String &label, Int32 value);

    Int32 value() const;
    void setValue(Int32 value);

public /*signals*/:

    Signal<Int32> onValueChanged{this};

private:

    class QComboBox *m_cb;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DROPDOWNPROPERTY_H

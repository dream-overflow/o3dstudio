/**
 * @brief Panel property abstract model.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-21
 * @details
 */

#ifndef _O3DS_COMMON_PANELPROPERTY_H
#define _O3DS_COMMON_PANELPROPERTY_H

#include <QtWidgets/QWidget>
#include "../panel.h"
#include <o3d/core/evthandler.h>
#include <o3d/core/evt.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Panel property abstract model.
 */
class O3S_API PanelProperty : public EvtHandler
{
public:

    //! Constructor
    PanelProperty(Panel *panel, const String &label);

    //! Destructor
    virtual ~PanelProperty();

    //! Get the resulting widget.
    QWidget* ui();

private:

    Panel *m_panel;
    Int32 m_index;
    QWidget *m_widget;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PANELPROPERTY_H

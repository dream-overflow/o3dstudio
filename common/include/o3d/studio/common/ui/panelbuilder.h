/**
 * @brief Toolset to construct a panel with properties.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-21
 * @details
 */

#ifndef _O3DS_COMMON_PANELBUILDER_H
#define _O3DS_COMMON_PANELBUILDER_H

#include <QtWidgets/QWidget>
#include "panel.h"

namespace o3d {
namespace studio {
namespace common {

class PanelProperty;

class O3S_API PanelBuilder
{
public:

    //! Constructor
    PanelBuilder(Panel *panel, const String &label);

    //! Destructor
    ~PanelBuilder();

    //! Get the resulting widget.
    QWidget* ui();

    void addPanelProperty(PanelProperty *pp);

private:

    Panel *m_panel;
    QWidget *m_widget;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PANELBUILDER_H

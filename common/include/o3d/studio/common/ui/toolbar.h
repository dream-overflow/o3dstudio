/**
 * @brief Common UI toolbar element
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_TOOLBAR_H
#define _O3DS_COMMON_TOOLBAR_H

#include <QtWidgets/QToolBar>
#include "../element.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API ToolBar : public Element
{
public:

    virtual QToolBar* ui() = 0;
    virtual Qt::ToolBarArea toolBarArea() const = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TOOLBAR_H

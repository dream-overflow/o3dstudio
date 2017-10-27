/**
 * @brief Common UI toolbar element
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_TOOLBAR_H
#define _O3DS_COMMON_TOOLBAR_H

#include <QtWidgets/QToolBar>

namespace o3d {
namespace studio {
namespace common {

class ToolBar
{
public:

    virtual QToolBar* ui() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TOOLBAR_H

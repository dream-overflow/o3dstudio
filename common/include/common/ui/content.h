/**
 * @brief Common UI content element
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_CONTENT_H
#define _O3DS_COMMON_CONTENT_H

#include <QtWidgets/QWidget>

namespace o3d {
namespace studio {
namespace common {

class Content
{
public:

    virtual QWidget* ui() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CONTENT_H

/**
 * @brief Common UI content element
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_CONTENT_H
#define _O3DS_COMMON_CONTENT_H

#include <QtWidgets/QWidget>
#include "../element.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API Content : public Element
{
public:

    virtual QWidget* ui() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CONTENT_H

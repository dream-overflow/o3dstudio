/**
 * @brief Common registerable named element
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_ELEMENT_H
#define _O3DS_COMMON_ELEMENT_H

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API Element
{
public:

    virtual ~Element() = 0;

    virtual String elementName() const = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ELEMENT_H

/**
 * @brief Common settings capacity interface
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-22
 * @details
 */

#ifndef _O3DS_COMMON_CAPACITYSETTING_H
#define _O3DS_COMMON_CAPACITYSETTING_H

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API CapacitySettings
{
public:

    virtual bool applySettings() = 0;
    virtual bool commitSettings() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CAPACITYSETTING_H

/**
 * @file commontype.h
 * @brief Enumerations for comon O3D objects types.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-02
 * @copyright Copyright (c) 2001-2017 Dream Overflow. All rights reserved.
 * @details
 */

#ifndef _O3DS_COMMON_TYPE_H
#define _O3DS_COMMON_TYPE_H

#include <o3d/core/coretype.h>

namespace o3d {
namespace studio {
namespace common {

enum CommonObjectType
{
    COMMON_BASE = USER_OBJECT,

    MASTER_SCENE_DRAWER,
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_TYPE_H

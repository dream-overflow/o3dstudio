/**
 * @brief Common module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_CAPACITY_H
#define _O3DS_COMMON_CAPACITY_H

namespace o3d {
namespace studio {
namespace common {

enum Capacity {
    CAPACITY_UI,
    CAPACITY_UI_SETTING,
    CAPACITY_TOOL,
    CAPACITY_RENDERER,
    CAPACITY_BUILDER,
    CAPACITY_IMPORTER,
    CAPACITY_NODE,
    CAPACITY_RESOURCE
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CAPACITY_H

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
    CAPACITY_UI = 0x00000001,
    CAPACITY_UI_SETTING = 0x00000002,
    CAPACITY_TOOL = 0x00000101,
    CAPACITY_RENDERER = 0x00000201,
    CAPACITY_EXPORTER = 0x00000301,
    CAPACITY_IMPORTER = 0x00000302,
    CAPACITY_NODE = 0x00000601,
    CAPACITY_RESOURCE = 0x00000701
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CAPACITY_H

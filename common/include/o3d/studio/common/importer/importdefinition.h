/**
 * @brief Common importer definition interface.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#ifndef _O3DS_COMMON_DEFINITION_H
#define _O3DS_COMMON_DEFINITION_H

#include <o3d/core/string.h>
#include <o3d/core/datetime.h>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Contains global definition of the imported data like creation timestamp,
 * creator, units, and number of differents type of imported objects.
 * Must be implemented per each importer.
 * At this level all data are transformed to by Y+ up axis, Z+ front axis,
 * in OpenGL style base for vertices data, and column majors matrices.
 */
class O3S_API ImportDefinition
{
public:

    virtual ~ImportDefinition() = 0;

    virtual String creator() const = 0;
    virtual DateTime creationDateTime() const = 0;
    virtual Float unit() const = 0;

    virtual UInt32 numModel() const = 0;
    virtual UInt32 numGeometry() const = 0;
    virtual UInt32 numMaterial() const = 0;
    virtual UInt32 numCamera() const = 0;
    virtual UInt32 numLight() const = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_IMPORTER_H

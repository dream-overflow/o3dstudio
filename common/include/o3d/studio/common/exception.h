/**
 * @brief Common base exceptions
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_EXCEPTION_H
#define _O3DS_COMMON_EXCEPTION_H

#include <o3d/core/error.h>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The BaseException class
 */
class O3S_API E_CommonException : public E_BaseException
{
    O3D_E_DEF_CLASS(E_CommonException)

    //! Ctor
    E_CommonException(const String& msg) throw() : E_BaseException(msg)
        O3D_E_DEF(E_CommonException, "Objective-3D Studio base exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_EXCEPTION_H

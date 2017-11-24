/**
 * @brief Application property section interface
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-21
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYSECTION_H
#define _O3DS_COMMON_PROPERTYSECTION_H

#include <QtWidgets/QtWidgets>

#include <o3d/core/string.h>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API PropertySection
{
public:

    virtual ~PropertySection() = 0;

    virtual String name() = 0;
    virtual String label() = 0;

    virtual Bool setupSection(QWidget *parent) = 0;
    virtual Bool cleanupSection(QWidget *parent) = 0;

    virtual void commit() = 0;
    virtual Bool save() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYSECTION_H

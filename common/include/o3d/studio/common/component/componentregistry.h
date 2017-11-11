/**
 * @brief Common component registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-10
 * @details
 */

#ifndef _O3DS_COMMON_COMPONENTREGISTRY_H
#define _O3DS_COMMON_COMPONENTREGISTRY_H

#include <QtWidgets/QWidget>
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API Component;

class ComponentRegistry
{
public:

    ComponentRegistry();
    virtual ~ComponentRegistry();

signals:

protected:


};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMPONENTREGISTRY_H

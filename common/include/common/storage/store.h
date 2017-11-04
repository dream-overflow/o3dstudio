/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_STORE_H
#define _O3DS_COMMON_STORE_H

#include <QtCore/QMap>
#include "storeitem.h"

namespace o3d {
namespace studio {
namespace common {

class Store
{
public:

    Store();
    ~Store();



protected:

    QMap<QUuid, StoreItem*> m_items;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STORE_H

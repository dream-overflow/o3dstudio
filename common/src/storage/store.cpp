/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-04
 * @details
 */

#include "common/storage/store.h"

using namespace o3d::studio::common;

Store::Store()
{

}

Store::~Store()
{
    StoreItem *storeItem = nullptr;
    foreach (storeItem, m_items) {
        delete storeItem;
    }
}

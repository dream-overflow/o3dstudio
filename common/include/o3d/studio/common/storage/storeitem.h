/**
 * @brief Common item for the storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_STOREITEM_H
#define _O3DS_COMMON_STOREITEM_H

#include "../workspace/project.h"
#include "../objectref.h"

#include <o3d/core/templatearray.h>

namespace o3d {
namespace studio {
namespace common {

class Project;

class O3S_API StoreItem
{
public:

    enum StoreItemState
    {
        STATE_NOT_STORED = 0,
        STATE_STORED,
        STATE_TRASHED,
        STATE_DELETED
    };

    StoreItem(Project *project);
    ~StoreItem();

    const String& name() const;
    const ObjectRef& ref() const;

    Project* project();
    const Project* project() const;

    const String& originalFullName() const;
    const ArrayChar& originalChecksum() const;

    StoreItemState storeItemState() const;

    /**
     * @brief Build the store item full filename
     * @return
     */
    String itemFileName() const;

    /**
     * @brief Put the resource data to project trash
     */
    void removeItem();

    /**
     * @brief Fully delete the resource content from the file system
     */
    void deleteItem();

protected:

    StoreItemState m_itemState;

    ObjectRef m_ref;                    //!< Unique object identifier
    String m_name;                      //!< User item name

    Project *m_project;                 //!< Owner project

    String m_originalFullName;          //!< Original file with full path (relative or absolute, as imported)
    ArrayChar m_originalChecksum;       //!< Original file SHA-1 checksum
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STOREITEM_H

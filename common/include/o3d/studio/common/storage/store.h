/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_STORE_H
#define _O3DS_COMMON_STORE_H

#include <QtCore/QCoreApplication>

#include "../exception.h"
#include "storeitem.h"

#include <map>
#include <list>

namespace o3d {
namespace studio {
namespace common {

class O3S_API Store
{
    Q_DECLARE_TR_FUNCTIONS(Store)

public:

    enum Version
    {
        STORE_VERSION_1_0_0 = 0x010000,
        STORE_VERSION_LATEST = STORE_VERSION_1_0_0
    };

    Store();
    ~Store();

    /**
     * @brief Init the storage for a new project. Create project structure and initials files.
     * @param project
     * @return
     */
    void initProject(Project *project, Version version = STORE_VERSION_LATEST);

    /**
     * @brief Setup storage for an existing project. Recreate missings directories and files as possible.
     * @param project
     * @return
     */
    void loadProject(Project *project);

    /**
     * @brief Erase all project files and items. Delete project folder if empty (no user manualy created files).
     * @param project
     * @return
     */
    void deleteProject(Project *project);

    /**
     * @brief Save storage for an existing project. Recreate missings directories and files as possible.
     * @param project
     * @return
     */
    void saveProject(Project *project);

    StoreItem* item(const Uuid &uuid);
    const StoreItem* item(const Uuid &uuid) const;

    StoreItem* addItem(const String &originalFileName);
    const StoreItem* addItem(const String &originalFileName) const;

    Bool removeItem(const Uuid &uuid);
    Bool removeItem(StoreItem *item);

    Bool deleteItem(const Uuid &uuid);
    Bool deleteItem(StoreItem *item);

    void purgeTrash(Project *project);

    std::list<StoreItem *> removedItems(Project *project);

protected:

    std::map<Uuid, StoreItem*> m_items;
};

/**
 * @brief The StoreException class
 */
class E_StoreException : public E_CommonException
{
    O3D_E_DEF_CLASS(E_StoreException)

    //! Ctor
    E_StoreException(const String& msg) throw() : E_CommonException(msg)
        O3D_E_DEF(E_StoreException, "Objective-3D Studio store exception")
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STORE_H

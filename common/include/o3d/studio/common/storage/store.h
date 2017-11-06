/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_STORE_H
#define _O3DS_COMMON_STORE_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "../exception.h"
#include "storeitem.h"

namespace o3d {
namespace studio {
namespace common {

class Store
{
    Q_DECLARE_TR_FUNCTIONS(Store)

public:

    Store();
    ~Store();

    /**
     * @brief Init the storage for a new project.
     * @param project
     * @return
     */
    void initProject(Project *project);

    /**
     * @brief Setup storage for an existing project.
     * @param project
     * @return
     */
    void loadProject(Project *project);

    /**
     * @brief Erase all project files and items.
     * @param project
     * @return
     */
    void deleteProject(Project *project);

    StoreItem* item(const QUuid &uuid);
    const StoreItem* item(const QUuid &uuid) const;

    StoreItem* addItem(const QString &originalFileName);
    const StoreItem* addItem(const QString &originalFileName) const;

    bool removeItem(const QUuid &uuid);
    bool removeItem(StoreItem *item);

    bool deleteItem(const QUuid &uuid);
    bool deleteItem(StoreItem *item);

    void purgeTrash(Project *project);

    QList<StoreItem*> removedItems(Project *project);

protected:

    QMap<QUuid, StoreItem*> m_items;
};

/**
 * @brief The StoreException class
 */
class StoreException : public BaseException
{
public:

    StoreException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STORE_H
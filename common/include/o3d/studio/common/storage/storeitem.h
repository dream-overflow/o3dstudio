/**
 * @brief Common item for the storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_STOREITEM_H
#define _O3DS_COMMON_STOREITEM_H

#include <QtCore/QFile>

#include "../workspace/project.h"
#include "../objectref.h"

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

    const QString& name() const;
    const ObjectRef& ref() const;

    Project* project();
    const Project* project() const;

    const QString& originalFullName() const;
    const QByteArray& originalChecksum() const;

    StoreItemState storeItemState() const;

    /**
     * @brief Build the store item full filename
     * @return
     */
    QString itemFileName() const;

    QFile &file();

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
    QString m_name;                     //!< User item name

    Project *m_project;                 //!< Owner project

    QString m_originalFullName;         //!< Original file with full path (relative or absolute, as imported)
    QByteArray m_originalChecksum;      //!< Original file SHA-1 checksum

    QFile m_file;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_STOREITEM_H

/**
 * @brief Common item for the storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#include "o3d/studio/common/storage/storeitem.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QFile>

using namespace o3d::studio::common;

// Returns empty QByteArray() on failure. @todo
/*QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}*/


StoreItem::StoreItem(Project* project) :
    m_ref(),
    m_project(project)
{
    O3D_ASSERT(project != nullptr);

    m_ref = ObjectRef::buildRef(project, TypeRef());  // @todo store ref type
}

StoreItem::~StoreItem()
{

}

const o3d::String &StoreItem::name() const
{
    return m_name;
}

const ObjectRef &StoreItem::ref() const
{
    return m_ref;
}

const Project *StoreItem::project() const
{
    return m_project;
}

const o3d::String &StoreItem::originalFullName() const
{
    return m_originalFullName;
}

const o3d::ArrayChar &StoreItem::originalChecksum() const
{
    return m_originalChecksum;
}

StoreItem::StoreItemState StoreItem::storeItemState() const
{
    return m_itemState;
}

o3d::String StoreItem::itemFileName() const
{
    String filename;
    Char s = '/';
    String hash1, hash2;

    const Uuid &uuid = m_ref.uuid();

    // generate two levels of path from the uuid node
    hash1 = String::print("%02x", (uuid.node()[0] << 16 & uuid.node()[1] << 8 & uuid.node()[2]) % 256);
    hash2 = String::print("%02x", (uuid.node()[3] << 16 & uuid.node()[4] << 8 & uuid.node()[5]) % 256);

    filename = m_project->path().getFullPathName() + s + "data" + hash1 + s + hash2 + s + uuid.toString() + ".content";

    return filename;
}

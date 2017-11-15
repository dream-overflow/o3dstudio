/**
 * @brief Common item for the storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#include "o3d/studio/common/storage/storeitem.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>

using namespace o3d::studio::common;

// Returns empty QByteArray() on failure.
QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}


StoreItem::StoreItem(Project* project) :
    m_ref(),
    m_project(project)
{
    Q_ASSERT(project != nullptr);

    m_ref = ObjectRef::buildRef(project, TypeRef());  // @todo store ref type
}

StoreItem::~StoreItem()
{

}

const QString &StoreItem::name() const
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

const QString &StoreItem::originalFullName() const
{
    return m_originalFullName;
}

const QByteArray &StoreItem::originalChecksum() const
{
    return m_originalChecksum;
}

StoreItem::StoreItemState StoreItem::storeItemState() const
{
    return m_itemState;
}

QString StoreItem::itemFileName() const
{
    QString filename;
    QChar s = QDir::separator();
    QString hash1, hash2;

    const QUuid &uuid = m_ref.uuid();

    // generate two levels of path from the uuid node
    hash1 = QString::asprintf("%02x", (uuid.data4[2] << 16 & uuid.data4[3] << 8 & uuid.data4[4]) % 256);
    hash2 = QString::asprintf("%02x", (uuid.data4[5] << 16 & uuid.data4[6] << 8 & uuid.data4[7]) % 256);

    filename = m_project->path().absolutePath() + s + "data" + hash1 + s + hash2 + s + uuid.toString() + ".content";

    return filename;
}

QFile& StoreItem::file()
{
    // @todo depending on state ?
    m_file.setFileName(itemFileName());

    return m_file;
}

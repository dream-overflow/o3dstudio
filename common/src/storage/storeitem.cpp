/**
 * @brief Common item for the storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-04
 * @details
 */

#include "common/storage/storeitem.h"
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
    m_project(project)
{
    Q_ASSERT(project != nullptr);
}

StoreItem::~StoreItem()
{

}

const QString &StoreItem::name() const
{
    return m_name;
}

const QUuid &StoreItem::uuid() const
{
    return m_uuid;
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

    // generate two levels of path from the uuid node
    hash1 = QString::asprintf("%02x", (m_uuid.data4[2] << 16 & m_uuid.data4[3] << 8 & m_uuid.data4[4]) % 256);
    hash2 = QString::asprintf("%02x", (m_uuid.data4[5] << 16 & m_uuid.data4[6] << 8 & m_uuid.data4[7]) % 256);

    filename = m_project->path().absolutePath() + s + "data" + hash1 + s + hash2 + s + m_uuid.toString() + ".content";

    return filename;
}

QFile& StoreItem::file()
{
    // @todo depending on state ?
    m_file.setFileName(itemFileName());

    return m_file;
}

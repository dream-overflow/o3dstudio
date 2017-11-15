/**
 * @brief Common project info data
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTINFO_H
#define _O3DS_COMMON_PROJECTINFO_H

#include "../global.h"

#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QDataStream>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Project info data
 */
class O3S_API ProjectInfo final
{
public:

    enum RevisionState
    {
        STATE_DEVELOPMENT = 0,
        STATE_ALPHA,
        STATE_BETA,
        STATE_RC1,
        STATE_RC2,
        STATE_RC3,
        STATE_RC4,
        STATE_RELEASE
    };

    ProjectInfo();
    ~ProjectInfo();

    QString& verboseName() { return m_verboseName; }
    const QString& verboseName() const { return m_verboseName; }

    QString& description() { return m_description; }
    const QString& description() const { return m_description; }

    QString& copyright() { return m_copyright; }
    const QString& copyright() const { return m_copyright; }

    QString& vendor() { return m_vendor; }
    const QString& vendor() const { return m_vendor; }

    QStringList& authors() { return m_authors; }
    const QStringList& authors() const { return m_authors; }

    QString& version() { return m_version; }
    const QString& version() const { return m_version; }

    qint32& revision() { return m_revision; }
    const qint32& revision() const { return m_revision; }

    qint32& revisionState() { return m_revisionState; }
    const qint32& revisionState() const { return m_revisionState; }

    QDateTime& creationDate() { return m_creationDate; }
    const QDateTime& creationDate() const { return m_creationDate; }

    QDateTime& modificationDate() { return m_modificationDate; }
    const QDateTime& modificationDate() const { return m_modificationDate; }

private:

    QString m_verboseName;
    QString m_description;
    QString m_copyright;
    QString m_vendor;
    QStringList m_authors;
    QString m_version;
    qint32 m_revision;
    qint32 m_revisionState;
    QDateTime m_creationDate;
    QDateTime m_modificationDate;

public:

    friend QDataStream& operator<<(QDataStream& stream, const ProjectInfo &projectInfo)
    {
        stream << projectInfo.verboseName()
               << projectInfo.description()
               << projectInfo.copyright()
               << projectInfo.vendor()
               << projectInfo.authors()
               << projectInfo.version()
               << projectInfo.revision()
               << projectInfo.revisionState()
               << projectInfo.creationDate()
               << projectInfo.modificationDate();

        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, ProjectInfo &projectInfo)
    {
        stream >> projectInfo.verboseName()
                >> projectInfo.description()
                >> projectInfo.copyright()
                >> projectInfo.vendor()
                >> projectInfo.authors()
                >> projectInfo.version()
                >> projectInfo.revision()
                >> projectInfo.revisionState()
                >> projectInfo.creationDate()
                >> projectInfo.modificationDate();

        return stream;
    }
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTINFO_H

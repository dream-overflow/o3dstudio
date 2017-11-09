/**
 * @brief Common project info data
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/projectinfo.h"

using namespace o3d::studio::common;

ProjectInfo::ProjectInfo() :
    m_version("1.0.0"),
    m_revision(1),
    m_revisionState(STATE_DEVELOPMENT)
{
    m_creationDate = m_modificationDate = QDateTime::currentDateTime();
}

ProjectInfo::~ProjectInfo()
{

}
/*
QDataStream& operator<<(QDataStream& stream, const ProjectInfo &projectInfo)
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
*/
/*QDataStream& operator>>(QDataStream& stream, ProjectInfo &projectInfo)
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
*/

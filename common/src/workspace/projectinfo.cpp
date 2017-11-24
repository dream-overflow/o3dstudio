/**
 * @brief Common project info data
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/workspace/projectinfo.h"

#include <o3d/core/instream.h>
#include <o3d/core/outstream.h>

using namespace o3d::studio::common;

ProjectInfo::ProjectInfo() :
    m_version("1.0.0"),
    m_revision(1),
    m_revisionState(STATE_DEVELOPMENT)
{
    m_creationDate = m_modificationDate = DateTime(True);
}

ProjectInfo::~ProjectInfo()
{

}

o3d::Bool ProjectInfo::writeToFile(OutStream &os)
{
    os << m_verboseName
       << m_description
       << m_copyright
       << m_vendor;

    os << Int32(m_authors.size());
    for (auto it = m_authors.begin(); it != m_authors.end(); ++it) {
       os << *it;
    }

    os << m_version
       << m_revision
       << m_revisionState
       << m_creationDate
       << m_modificationDate;

    return True;
}

o3d::Bool ProjectInfo::readFromFile(o3d::InStream &is)
{
   is >> m_verboseName
           >> m_description
           >> m_copyright
           >> m_vendor;

   Int32 num;
   String author;
   is >> num;
   for (Int32 i = 0; i < num; ++i) {
       is >> author;
       m_authors.push_back(author);
   }

   is >> m_version
           >> m_revision
           >> m_revisionState
           >> m_creationDate
           >> m_modificationDate;

   return True;
}

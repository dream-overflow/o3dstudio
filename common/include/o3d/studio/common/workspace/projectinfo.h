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

#include <o3d/core/datetime.h>
#include <o3d/core/stringlist.h>

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

    String& verboseName() { return m_verboseName; }
    const String& verboseName() const { return m_verboseName; }

    String& description() { return m_description; }
    const String& description() const { return m_description; }

    String& copyright() { return m_copyright; }
    const String& copyright() const { return m_copyright; }

    String& vendor() { return m_vendor; }
    const String& vendor() const { return m_vendor; }

    T_StringList& authors() { return m_authors; }
    const T_StringList& authors() const { return m_authors; }

    String& version() { return m_version; }
    const String& version() const { return m_version; }

    Int32& revision() { return m_revision; }
    const Int32& revision() const { return m_revision; }

    Int32& revisionState() { return m_revisionState; }
    const Int32& revisionState() const { return m_revisionState; }

    DateTime& creationDate() { return m_creationDate; }
    const DateTime& creationDate() const { return m_creationDate; }

    DateTime& modificationDate() { return m_modificationDate; }
    const DateTime& modificationDate() const { return m_modificationDate; }

    Bool writeToFile(OutStream &os);
    Bool readFromFile(InStream &is);

private:

    String m_verboseName;
    String m_description;
    String m_copyright;
    String m_vendor;
    T_StringList m_authors;
    String m_version;
    Int32 m_revision;
    Int32 m_revisionState;
    DateTime m_creationDate;
    DateTime m_modificationDate;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECTINFO_H

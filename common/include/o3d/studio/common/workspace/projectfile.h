/**
 * @brief Common project file object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-05
 * @details
 */

#ifndef _O3DS_COMMON_PROJECTFILE_H
#define _O3DS_COMMON_PROJECTFILE_H

#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

#include "../global.h"

class QDataStream;

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Project file
 */
class O3S_API ProjectFile
{
    Q_DECLARE_TR_FUNCTIONS(ProjectFile)

public:

    enum ProjectVersion
    {
        PROJECT_VERSION_1_0_0 = 0x010000,
        PROJECT_VERSION_LATEST = PROJECT_VERSION_1_0_0

    };

    ProjectFile(Project *project, ProjectVersion version = PROJECT_VERSION_1_0_0);
    virtual ~ProjectFile();

    const QString& name() const;
    QString filename() const;
    const QDir& path() const;

    /**
     * @brief Initialize a new project at the specified path with name.
     */
    void create();

    void load();
    void save();

private:

    Project *m_project;        //!< Owner project
    ProjectVersion m_version;  //!< Version

protected:

    static const char PROJECT_MAGIC[];
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

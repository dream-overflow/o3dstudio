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

class QDataStream;

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Project file
 */
class ProjectFile
{
    Q_DECLARE_TR_FUNCTIONS(ProjectFile)

public:

    enum ProjectVersion
    {
        PF_100 = 0x010000,
        PF_101 = 0x010001,
        PF_110 = 0x010100,
        PF_111 = 0x010101
    };

    ProjectFile(Project *project, ProjectVersion version = PF_100);
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

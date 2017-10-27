/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_PROJECT_H
#define _O3DS_COMMON_PROJECT_H

#include <QtCore/QString>

namespace o3d {
namespace studio {
namespace common {

class Workspace;

/**
 * @brief The Project final class
 */
class Project
{
public:

    Project(Workspace *workspace, const QString &name);
    ~Project();

    const QString& getName() const;
    const QString& getFilename() const;

    bool setLocation(const QString &path);

    bool load();
    bool save();

private:

    Workspace *m_workspace;    //!< Related workspace;

    QString m_filename;        //!< Related project file name
    QString m_name;            //!< Unique project name
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

/**
 * @brief Common workspace object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_WORKSPACE_H
#define _O3DS_COMMON_WORKSPACE_H

#include <QtCore/QStringList>
#include <QtCore/QMap>

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Workspace final class.
 */
class Workspace
{
public:

    Workspace(const QString &name);
    ~Workspace();

    const QString& getName() const;
    const QString& getFilename() const;

    Project* getProject(const QString& name);
    const Project* getProject(const QString& name) const;

    /**
     * @brief Return the list of found projects.
     * @return
     */
    QStringList getProjectsList() const;

    Workspace* addWorkspace(const QString &name);
    Workspace* getWorkspace(const QString &name);
    bool deleteWorkspace(const QString &name);
    bool hasWorkspace(const QString &name);

    /**
     * @brief Changes occurs to one or more projects.
     * @return
     */
    bool hasChanged() const;

    /**
     * @brief Set the current selected workspace.
     * @param name
     */
    bool selectWorkspace(const QString &name);

    bool save();
    bool load();

private:

    QString m_filename;        //!< Related workspace file name
    QString m_name;            //!< Unique workspace name

    QStringList m_foundProjects;
    QMap<QString, Project*> m_loadedProjects;

    Project *m_selectedProject{nullptr};
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACE_H

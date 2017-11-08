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
#include <QtCore/QUuid>

#include "../exception.h"

namespace o3d {
namespace studio {
namespace common {

class Project;

/**
 * @brief The Workspace final class.
 */
class Workspace : public QObject
{
    Q_OBJECT

public:

    Workspace(const QString &name, QObject *parent = nullptr);
    virtual ~Workspace();

    const QString& name() const;
    const QString& filename() const;

    Project* project(const QUuid& uuid);
    const Project* project(const QUuid& uuid) const;

    /**
     * @brief Return the list of found projects.
     * @return
     */
    QStringList projectsList() const;

    bool addProject(Project *project);

    /**
     * @brief The project is saved before to be closed and removed from the workspace.
     * @param uuid
     * @return
     */
    bool closeProject(const QUuid& uuid);

    bool hasProject(const QUuid& uuid) const;
    bool hasProject(QString location) const;

    /**
     * @brief Changes occurs to one or more projects.
     * @return
     */
    bool hasChanges() const;

    /**
     * @brief Set the current selected project.
     * @param name
     */
    bool selectProject(const QUuid &uuid);

    bool save();
    bool load();

signals:

    void onProjectAdded(const QUuid &uuid);
    void onProjectActivated(const QUuid &uuid);
    void onProjectRemoved(const QUuid &uuid);

public slots:

    void onSelectionChanged();

private:

    QString m_filename;        //!< Related workspace file name
    QString m_name;            //!< Unique workspace name

    QStringList m_foundProjects;
    QMap<QUuid, Project*> m_loadedProjects;

    Project *m_activeProject{nullptr};
};

/**
 * @brief The WorkspaceException class
 */
class WorkspaceException : public BaseException
{
public:

    WorkspaceException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_WORKSPACE_H

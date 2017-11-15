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
#include "../objectref.h"

#include "project.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
class Messenger;

/**
 * @brief The Workspace final class.
 */
class O3S_API Workspace : public QObject
{
    Q_OBJECT

public:

    Workspace(const QString &name, QObject *parent = nullptr);
    virtual ~Workspace();

    const QUuid& uuid() const;
    const QString& name() const;
    const QString& filename() const;

    quint32 generateProjectId();

    void setUuid(const QUuid &uuid);

    Project* project(const LightRef& ref);
    const Project* project(const LightRef& ref) const;

    Project* activeProject();
    const Project* activeProject() const;

    /**
     * @brief Return the list of found projects.
     * @return
     */
    QStringList projectsList() const;

    /**
     * @brief loadedProjectList
     * @return
     */
    QList<Project*> loadedProjectList() const;

    /**
     * @brief addProject
     * @param project
     * @return
     */
    bool addProject(Project *project);

    /**
     * @brief The project is saved before to be closed and removed from the workspace.
     * @param uuid
     * @return
     */
    bool closeProject(const LightRef& ref);

    bool hasProject(const LightRef& ref) const;
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
    bool setActiveProject(const LightRef &ref);

    bool save();
    bool load();

    //
    // Project entity
    //

    /**
     * @brief Get a specific hub from a loaded project.
     */
    Hub* hub(const LightRef &ref);

    /**
     * @brief Get a specific hub from a loaded project (const version).
     */
    const Hub* hub(const LightRef &ref) const;

    /**
     * @brief Find a specific hub, recursively from a loaded project.
     */
    Hub* findHub(const LightRef &ref);

    /**
     * @brief Find a specific hub, recursively from a loaded project (const version).
     */
    const Hub* findHub(const LightRef &ref) const;

    /**
     * @brief Get a specific fragment from a loaded project.
     */
    Fragment* fragment(const LightRef &ref);

    /**
     * @brief Get a specific fragment from a loaded project (const version).
     */
    const Fragment* fragment(const LightRef &ref) const;

    /**
     * @brief Get a specific asset from a loaded project.
     */
    Asset* asset(const LightRef &ref);

    /**
     * @brief Get a specific asset from a loaded project (const version).
     */
    const Asset* asset(const LightRef &ref) const;

signals:

    void onProjectAdded(const LightRef &ref);
    void onProjectActivated(const LightRef &ref);
    void onProjectRemoved(const LightRef &ref);

    void onProjectHubAdded(const LightRef &ref);
    void onProjectHubRemoved(const LightRef &ref);

    void onProjectFragmentAdded(const LightRef &ref);
    void onProjectFragmentRemoved(const LightRef &ref);

    void onProjectAssetAdded(const LightRef &ref);
    void onProjectAssetRemoved(const LightRef &ref);

public slots:

    void onSelectionChanged();

private:

    QUuid m_uuid;
    quint32 m_nextId;

    QString m_filename;        //!< Related workspace file name
    QString m_name;            //!< Unique workspace name

    QStringList m_foundProjects;
    QMap<quint32, Project*> m_loadedProjects;

    Project *m_activeProject{nullptr};

    Messenger& messenger();
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

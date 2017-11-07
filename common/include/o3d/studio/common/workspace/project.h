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
#include <QtCore/QUuid>
#include <QtCore/QDir>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "../exception.h"

class QDataStream;

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class MasterScene;
class O3DCanvasContent;
class ProjectFile;

/**
 * @brief The Project final class
 */
class Project
{
    Q_DECLARE_TR_FUNCTIONS(Project)

public:

    Project(const QString &name, Workspace *workspace = nullptr);
    virtual ~Project();

    void setWorkspace(Workspace *workspace);

    Workspace* workspace();
    const Workspace* workspace() const;

    /**
     * @brief Initialize a new project at the specified path with name.
     * @param stream
     */
    void create();

    void setUuid(const QUuid &uuid);

    const QUuid& uuid() const;
    const QString& name() const;
    QString filename() const;
    const QDir& path() const;

    bool setLocation(const QDir &path);

    bool load();
    bool save();

    bool exists() const;
    bool hasChanges();

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    void setupMasterScene();

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    QString m_filename;        //!< Project file name
    QString m_name;            //!< Project display name
    QDir m_path;               //!< Project path

    ProjectFile *m_projectFile;

    QUuid m_uuid;              //!< Unique projet identifier

    MasterScene *m_masterScene;
};

/**
 * @brief The ProjectException class
 */
class ProjectException : public BaseException
{
public:

    ProjectException(const QString &message);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

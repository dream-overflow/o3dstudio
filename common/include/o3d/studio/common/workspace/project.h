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
#include "../objectref.h"

class QDataStream;

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class MasterScene;
class O3DCanvasContent;
class ProjectFile;
class ProjectInfo;

/**
 * @brief The Project final class
 */
class Project
{
    Q_DECLARE_TR_FUNCTIONS(Project)

    friend class ProjectFile;

public:

    Project(const QString &name, Workspace *workspace = nullptr);
    virtual ~Project();

    void setWorkspace(Workspace *workspace);

    Workspace* workspace();
    const Workspace* workspace() const;

    qint64 generateId();

    /**
     * @brief Initialize a new project at the specified path with name.
     * @param stream
     */
    void create();

    void setRef(const ObjectRef &ref);

    const QString& name() const;
    QString filename() const;
    const QDir& path() const;

    const ObjectRef& ref() const;

    bool setLocation(const QDir &path);

    const ProjectInfo& info() const;
    ProjectInfo& info();

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

    ObjectRef m_ref;
    qint64 m_nextId;

    ProjectInfo *m_info;
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

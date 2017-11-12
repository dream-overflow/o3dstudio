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
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "entity.h"

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
class Hub;
class Fragment;

/**
 * @brief The Project final class
 */
class O3S_API Project : public Entity
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
    virtual void create() override;

    QString filename() const;
    const QDir& path() const;

    bool setLocation(const QDir &path);

    const ProjectInfo& info() const;
    ProjectInfo& info();

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;
    virtual bool hasChanges() override;

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    void setupMasterScene();

    //
    // hub
    //

    void addHub(Hub *hub);

    void removeHub(const LightRef &ref);
    void removeHub(qint64 id);
    void removeHub(Hub *hub);

    Hub* hub(const LightRef &ref);
    const Hub* hub(const LightRef &ref) const;

    Hub* hub(qint64 id);
    const Hub* hub(qint64 id) const;

    QList<Hub*> searchHub(const QString &name);
    QList<const Hub*> searchHub(const QString &name) const;

    //
    // fragment
    //

    void addFragment(Fragment *fragment);

    void removeFragment(const LightRef &ref);
    void removeFragment(qint64 id);
    void removeFragment(Fragment *fragment);

    Fragment* fragment(const LightRef &ref);
    const Fragment* fragment(const LightRef &ref) const;

    Fragment* fragment(qint64 id);
    const Fragment* fragment(qint64 id) const;

    QList<Fragment*> searchFragment(const QString &name);
    QList<const Fragment*> searchFragment(const QString &name) const;

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    QString m_filename;        //!< Project file name
    QDir m_path;               //!< Project path

    ProjectFile *m_projectFile;

    qint64 m_nextId;

    ProjectInfo *m_info;
    MasterScene *m_masterScene;

    QMap<qint64, Fragment*> m_fragments;
    QMap<qint64, Hub*> m_hubs;
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

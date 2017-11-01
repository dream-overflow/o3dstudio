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

namespace o3d {
namespace studio {
namespace common {

class Workspace;
class MasterScene;
class O3DCanvasContent;

/**
 * @brief The Project final class
 */
class Project
{
public:

    Project(Workspace *workspace, const QString &name);
    ~Project();

    const QUuid& uuid() const;
    const QString& name() const;
    const QString& filename() const;
    const QDir& path() const;

    bool setLocation(const QString &path);

    bool load();
    bool save();

    bool hasChanges();

    MasterScene* masterScene();
    const MasterScene* masterScene() const;

    void initialize();

private:

    Workspace *m_workspace;    //!< Workspace where the projet is currently loaded

    QString m_filename;        //!< Project file name
    QString m_name;            //!< Project display name
    QDir m_path;               //!< Project path

    QUuid m_uuid;              //!< Unique projet identifier

    MasterScene *m_masterScene;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROJECT_H

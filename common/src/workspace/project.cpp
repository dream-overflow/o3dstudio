/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "common/application.h"
#include "common/storage/store.h"

#include "common/workspace/project.h"
#include "common/workspace/projectfile.h"
#include "common/workspace/workspace.h"
#include "common/workspace/masterscene.h"

using namespace o3d::studio::common;

Project::Project(const QString &name, Workspace *workspace) :
    m_workspace(workspace),
    m_filename(),
    m_name(name),
    m_path(QDir::current()),
    m_uuid(QUuid::createUuid()),
    m_masterScene(nullptr)
{
    m_masterScene = new MasterScene(this);
    m_projectFile = new ProjectFile(this, ProjectFile::PF_100);
}

Project::~Project()
{
    delete m_masterScene;
    delete m_projectFile;
}

void Project::setWorkspace(Workspace *workspace)
{
    m_workspace = workspace;
}

Workspace *Project::workspace()
{
    return m_workspace;
}

const Workspace *Project::workspace() const
{
    return m_workspace;
}

void Project::create()
{
    if (exists()) {
        throw ProjectException(tr("Project already exists"));
    };

    // project structure
    Application::instance()->store().initProject(this);

    // and project file
    m_projectFile->create();
}

const QUuid &Project::uuid() const
{
    return m_uuid;
}

const QString& Project::name() const
{
    return m_name;
}

const QString& Project::filename() const
{
    return m_filename;
}

const QDir &Project::path() const
{
    return m_path;
}

bool Project::setLocation(const QDir &path)
{
    if (!path.exists()) {
        return false;
    }

    m_path = path.absolutePath() + QDir::separator() + m_name;

    return true;
}

bool Project::load()
{
    return true;
}

bool Project::save()
{
    return true;
}

bool Project::exists() const
{
    return m_path.exists() && m_path.exists("project.o3dstudio");
}

bool Project::hasChanges()
{
    // @todo
    return true;
}

MasterScene *Project::masterScene()
{
    return m_masterScene;
}

const MasterScene *Project::masterScene() const
{
    return m_masterScene;
}

void Project::setupMasterScene()
{
    if (m_masterScene) {
        m_masterScene->initialize();
    }
}

ProjectException::ProjectException(const QString &message) :
    BaseException(message)
{

}

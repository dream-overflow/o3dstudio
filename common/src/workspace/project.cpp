/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/storage/store.h"

#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/projectinfo.h"
#include "o3d/studio/common/workspace/projectfile.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/masterscene.h"

using namespace o3d::studio::common;

Project::Project(const QString &name, Workspace *workspace) :
    Entity(nullptr),
    m_workspace(workspace),
    m_filename(),
    m_path(QDir::current()),
    m_nextId(1),
    m_info(nullptr),
    m_masterScene(nullptr)
{
    m_typeRef = TypeRef::project();

    m_name = name;
    m_ref = ObjectRef::buildRef(workspace);
    m_masterScene = new MasterScene(this);
    m_projectFile = new ProjectFile(this, ProjectFile::PF_100);
    m_info = new ProjectInfo();
}

Project::~Project()
{
    delete m_masterScene;
    delete m_projectFile;
    delete m_info;

    Fragment *fragment = nullptr;
    foreach (fragment, m_fragments) {
        delete fragment;
    }
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

qint64 Project::generateId()
{
    qint64 nextId = m_nextId++;
    return nextId;
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

QString Project::filename() const
{
    return m_path.absoluteFilePath("project.o3dstudio");
}

const QDir &Project::path() const
{
    return m_path;
}

const ProjectInfo &Project::info() const
{
    return *m_info;
}

ProjectInfo &Project::info()
{
    return *m_info;
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
    if (!exists()) {
        throw ProjectException(tr("Project doesn't exists"));
    };

    // project structure
    Application::instance()->store().loadProject(this);

    // and project file
    m_projectFile->load();

    // @todo save next id

    return true;
}

bool Project::save()
{
    if (!exists()) {
        throw ProjectException(tr("Project doesn't exists"));
    };

    // project structure @todo
    // Application::instance()->store().saveProject(this);

    // @todo if changes only
    // and project file
    m_projectFile->save();

    // @todo load next id

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

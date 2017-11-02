/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include "common/workspace/project.h"
#include "common/workspace/workspace.h"
#include "common/workspace/masterscene.h"

using namespace o3d::studio::common;

Project::Project(Workspace *workspace, const QString &name) :
    m_workspace(workspace),
    m_filename(),
    m_name(name),
    m_path(QDir::current()),
    m_uuid(QUuid::createUuid()),
    m_masterScene(nullptr)
{
    m_masterScene = new MasterScene(this);
}

Project::~Project()
{
    delete m_masterScene;
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

bool Project::setLocation(const QString &path)
{
    // @todo
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

void Project::initialize()
{
    if (m_masterScene) {
        m_masterScene->initialize();
    }
}

/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QDataStream>

#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/projectinfo.h"
#include "o3d/studio/common/workspace/projectfile.h"

using namespace o3d::studio::common;

const char ProjectFile::PROJECT_MAGIC[] = "O3DSTUDIO";


ProjectFile::ProjectFile(Project *project, ProjectFile::ProjectVersion version) :
    m_project(project),
    m_version(version)
{
    Q_ASSERT(project != nullptr);
}

ProjectFile::~ProjectFile()
{

}

const QString &ProjectFile::name() const
{
    return m_project->name();
}

QString ProjectFile::filename() const
{
    return m_project->path().absoluteFilePath("project.o3dstudio");
}

const QDir &ProjectFile::path() const
{
    return m_project->path();
}

void ProjectFile::create()
{
    save();
}

void ProjectFile::load()
{
    QFile file(m_project->path().absoluteFilePath("project.o3dstudio"));
    if (!file.exists()) {
        throw ProjectException(tr("Missing project file for %1").arg(m_project->name()));
    }

    file.open(QFile::ReadOnly);

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw ProjectException(tr("Project file streaming output not ok"));
    }

    // header
    char lmagic[9] = {0};

    const int magicLen = sizeof(PROJECT_MAGIC) - 1;

    int size = stream.readRawData(lmagic, magicLen);
    if ((size != magicLen) || (memcmp(lmagic, PROJECT_MAGIC, magicLen) != 0)) {
        throw ProjectException(tr("Invalid project file format"));
    }

    QUuid uuid;
    QString name;

    stream >> uuid
           >> name;

    m_project->setRef(ObjectRef::buildRef(m_project->workspace(), uuid));

    if (m_project->name() != name) {
        throw ProjectException(tr("Invalid project name"));
    }

    // id generator
    stream >> m_project->m_nextId;

    // global info
    stream >> *m_project->m_info;

    file.close();
}

void ProjectFile::save()
{
    QFile file(m_project->path().absoluteFilePath("project.o3dstudio"));
    file.open(QFile::WriteOnly | QFile::Truncate);

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw ProjectException(tr("Project file streaming output not ok"));
    }

    // header
    const int magicLen = sizeof(PROJECT_MAGIC) - 1;
    stream.writeRawData(PROJECT_MAGIC, magicLen);
    stream << m_project->ref().uuid()
           << m_project->name();

    // id generator
    stream << m_project->m_nextId;

    // global info
    // update modification date to now
    m_project->m_info->modificationDate() = QDateTime::currentDateTime();

    stream << *m_project->m_info;

    file.close();
}

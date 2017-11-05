/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QDataStream>

#include "common/workspace/project.h"
#include "common/workspace/projectfile.h"

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
    file.open(QFile::ReadOnly);

    file.setFileName(m_project->path().absoluteFilePath("project.o3dstudio"));
    if (!file.exists()) {
        throw ProjectException(m_project->tr("Missing project file for {1}").arg(m_project->name()));
    }

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw ProjectException(m_project->tr("Project file streaming output not ok"));
    }

    char lmagic[9] = {0};

    const int magicLen = sizeof(PROJECT_MAGIC) - 1;

    int size = stream.readRawData(lmagic, magicLen);
    if ((size != sizeof(PROJECT_MAGIC)) || (memcmp(lmagic, PROJECT_MAGIC, magicLen) != 0)) {

        throw ProjectException(m_project->tr("Invalid project file format"));
    }

    file.close();
}

void ProjectFile::save()
{
    QFile file(m_project->path().absoluteFilePath("project.o3dstudio"));
    file.open(QFile::WriteOnly | QFile::Truncate);

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw ProjectException(m_project->tr("Project file streaming output not ok"));
    }

    const int magicLen = sizeof(PROJECT_MAGIC) - 1;
    stream.writeRawData(PROJECT_MAGIC, magicLen);

    file.close();
}

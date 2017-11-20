/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/storage/store.h"
#include "o3d/studio/common/workspace/project.h"

#include <QtCore/QDataStream>
#include <QtCore/QFile>

using namespace o3d::studio::common;

Store::Store()
{

}

Store::~Store()
{
    StoreItem *storeItem = nullptr;
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        storeItem = it->second;
        delete storeItem;
    }
}

void Store::initProject(Project *project, Version version)
{
    if (!project) {
        return;
    }

    if (project->path().exists()) {
        throw E_StoreException(fromQString(tr("Project path exists")));
    }

    if (!Application::instance()->workspaces().defaultProjectsPath().makeDir(project->name())) {
        throw E_StoreException("Unable to create the project directory");
    }

    if (project->path().makeDir("items") != Dir::SUCCESS) {
        throw E_StoreException("Unable to create the project items directory");
    }

    if (project->path().makeDir("cache") != Dir::SUCCESS) {
        throw E_StoreException("Unable to create the project cache directory");
    }

    if (project->path().makeDir("tmp") != Dir::SUCCESS) {
        throw E_StoreException("Unable to create the project temporary directory");
    }

    if (project->path().makeDir("targets") != Dir::SUCCESS) {
        throw E_StoreException("Unable to create the project targets path");
    }

    QFile readmeFile(project->path().makeFullFileName("README.txt"));
    if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
        throw E_StoreException("Unable to create the project readme file");
    }
    readmeFile.write("TODO\n");
    readmeFile.close();

    QFile licenseFile(project->path().makeFullFileName("LICENSE.txt"));
    if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
        throw E_StoreException("Unable to create the project licnse file");
    }
    licenseFile.write("TODO\n");
    licenseFile.close();

    QFile infoFile(project->path().makeFullFileName("INFO.txt"));
    if (!infoFile.open(QFile::WriteOnly | QFile::Text)) {
        throw E_StoreException("Unable to create the project information file");
    }
    infoFile.write("TODO\n");
    infoFile.close();
}

void Store::loadProject(Project *project)
{
    if (!project) {
        return;
    }

    // check project structure, integrity
    if (!project->path().exists()) {
        throw E_StoreException(fromQString(tr("Project directory doesn't exists")));
    }

    if (project->path().check("items") != Dir::SUCCESS) {
        throw E_StoreException("Project items directory is missing");
    }

    if (project->path().check("cache") != Dir::SUCCESS) {
        O3D_WARNING("Project cache directory is missing");

        if (project->path().makeDir("cache") != Dir::SUCCESS) {
            throw E_StoreException("Unable to create the project cache directory");
        }
    }

    if (project->path().check("tmp") != Dir::SUCCESS) {
        O3D_WARNING("Project temporary directory is missing");

        if (project->path().makeDir("tmp") != Dir::SUCCESS) {
            throw E_StoreException("Unable to create the project temporary directory");
        }
    }

    if (project->path().check("targets") != Dir::SUCCESS) {
        throw E_StoreException("Project targets directory is missing");
    }

    if (project->path().check("README.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project readme file is missing");

        QFile readmeFile(project->path().makeFullFileName("README.txt"));
        if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project readme file");
        }
        readmeFile.write("TODO\n");
        readmeFile.close();
    }

    if (project->path().check("LICENSE.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project license file is missing");

        QFile licenseFile(project->path().makeFullFileName("LICENSE.txt"));
        if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project license file");
        }
        licenseFile.write("TODO\n");
        licenseFile.close();
    }

    if (project->path().check("INFO.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project information file is missing");

        QFile infoFile(project->path().makeFullFileName("INFO.txt"));
        if (!infoFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project information file");
        }
        infoFile.write("TODO\n");
        infoFile.close();
    }
}

void Store::saveProject(Project *project)
{
    if (!project) {
        return;
    }

    // check project structure, integrity
    if (!project->path().exists()) {
        throw E_StoreException(fromQString(tr("Project directory doesn't exists")));
    }

    if (project->path().check("items") != Dir::SUCCESS) {
        throw E_StoreException("Project items directory is missing");
    }

    if (project->path().check("cache") != Dir::SUCCESS) {
        O3D_WARNING("Project cache directory is missing");

        if (project->path().makeDir("cache") != Dir::SUCCESS) {
            throw E_StoreException("Unable to create the project cache directory");
        }
    }

    if (project->path().check("tmp") != Dir::SUCCESS) {
        O3D_WARNING("Project temporary directory is missing");

        if (project->path().makeDir("tmp") != Dir::SUCCESS) {
            throw E_StoreException("Unable to create the project temporary directory");
        }
    }

    if (project->path().check("targets") != Dir::SUCCESS) {
        throw E_StoreException("Project targets directory is missing");
    }

    if (project->path().check("README.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project readme file is missing");

        QFile readmeFile(project->path().makeFullFileName("README.txt"));
        if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project readme file");
        }
        readmeFile.write("TODO\n");
        readmeFile.close();
    }

    if (project->path().check("LICENSE.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project license file is missing");

        QFile licenseFile(project->path().makeFullFileName("LICENSE.txt"));
        if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project license file");
        }
        licenseFile.write("TODO\n");
        licenseFile.close();
    }

    if (project->path().check("INFO.txt") != Dir::SUCCESS) {
        O3D_WARNING("Project information file is missing");

        QFile infoFile(project->path().makeFullFileName("INFO.txt"));
        if (!infoFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project information file");
        }
        infoFile.write("TODO\n");
        infoFile.close();
    }
}

void Store::deleteProject(Project *project)
{
    if (!project) {
        return;
    }

    DiskDir path(project->path());

    if (!path.exists()) {
        throw E_StoreException(fromQString(tr("Project directory doesn't exists")));
    }

    // items
    if (path.check("items") == Dir::SUCCESS) {
        // @todo delete all its content

        path.removeDir("items");
    }

    // cache
    if (path.check("cache") == Dir::SUCCESS) {
        // @todo delete all its content

        path.removeDir("cache");
    }

    // tmp
    if (path.check("tmp") == Dir::SUCCESS) {
        // @todo delete all its content

        path.removeDir("tmp");
    }

    // targets
    if (path.check("targets") == Dir::SUCCESS) {
        // @todo delete all its content

        path.removeDir("targets");
    }

    // README
    if (path.check("README.txt") == Dir::SUCCESS) {
        path.removeFile("README.txt");
    }

    // LICENSE
    if (path.check("LICENSE.txt") == Dir::SUCCESS) {
        path.removeFile("LICENSE.txt");
    }

    // INFO
    if (path.check("INFO.txt") == Dir::SUCCESS) {
        path.removeFile("INFO.txt");
    }

    // project directory himself if empty
    if (path.empty() && path.cdUp()) {
        path.removeDir(project->name());
    }
}

StoreItem *Store::item(const QUuid &uuid)
{
    StoreItem *item = nullptr;

    return item;
}

const StoreItem *Store::item(const QUuid &uuid) const
{
    const StoreItem *item = nullptr;

    return item;
}

StoreItem *Store::addItem(const String &originalFileName)
{
    StoreItem *item = nullptr;

    return item;
}

const StoreItem *Store::addItem(const String &originalFileName) const
{
    StoreItem *item = nullptr;

    return item;
}

o3d::Bool Store::removeItem(const QUuid &uuid)
{
    return False;
}

o3d::Bool Store::removeItem(StoreItem *item)
{
    return False;
}

o3d::Bool Store::deleteItem(const QUuid &uuid)
{
    return False;
}

o3d::Bool Store::deleteItem(StoreItem *item)
{
    return False;
}

void Store::purgeTrash(Project *project)
{

}

std::list<StoreItem *> Store::removedItems(Project *project)
{
    std::list<StoreItem*> items;

    return items;
}

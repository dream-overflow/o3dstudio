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

using namespace o3d::studio::common;

Store::Store()
{

}

Store::~Store()
{
    StoreItem *storeItem = nullptr;
    foreach (storeItem, m_items) {
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

    if (!Application::instance()->workspaces().defaultProjectsPath().mkdir(project->name())) {
        throw E_StoreException("Unable to create the project directory");
    }

    if (!project->path().mkdir("items")) {
        throw E_StoreException("Unable to create the project items directory");
    }

    if (!project->path().mkdir("cache")) {
        throw E_StoreException("Unable to create the project cache directory");
    }

    if (!project->path().mkdir("tmp")) {
        throw E_StoreException("Unable to create the project temporary directory");
    }

    if (!project->path().mkdir("targets")) {
        throw E_StoreException("Unable to create the project targets path");
    }

    QFile readmeFile(project->path().absoluteFilePath("README.txt"));
    if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
        throw E_StoreException("Unable to create the project readme file");
    }
    readmeFile.write("TODO\n");
    readmeFile.close();

    QFile licenseFile(project->path().absoluteFilePath("LICENSE.txt"));
    if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
        throw E_StoreException("Unable to create the project licnse file");
    }
    licenseFile.write("TODO\n");
    licenseFile.close();

    QFile infoFile(project->path().absoluteFilePath("INFO.txt"));
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

    if (!project->path().exists("items")) {
        throw E_StoreException("Project items directory is missing");
    }

    if (!project->path().exists("cache")) {
        qWarning("Project cache directory is missing");

        if (!project->path().mkdir("cache")) {
            throw E_StoreException("Unable to create the project cache directory");
        }
    }

    if (!project->path().exists("tmp")) {
        qWarning("Project temporary directory is missing");

        if (!project->path().mkdir("tmp")) {
            throw E_StoreException("Unable to create the project temporary directory");
        }
    }

    if (!project->path().exists("targets")) {
        throw E_StoreException("Project targets directory is missing");
    }

    if (!project->path().exists("README.txt")) {
        qWarning("Project readme file is missing");

        QFile readmeFile(project->path().absoluteFilePath("README.txt"));
        if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project readme file");
        }
        readmeFile.write("TODO\n");
        readmeFile.close();
    }

    if (!project->path().exists("LICENSE.txt")) {
        qWarning("Project license file is missing");

        QFile licenseFile(project->path().absoluteFilePath("LICENSE.txt"));
        if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project license file");
        }
        licenseFile.write("TODO\n");
        licenseFile.close();
    }

    if (!project->path().exists("INFO.txt")) {
        qWarning("Project information file is missing");

        QFile infoFile(project->path().absoluteFilePath("INFO.txt"));
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

    if (!project->path().exists("items")) {
        throw E_StoreException("Project items directory is missing");
    }

    if (!project->path().exists("cache")) {
        qWarning("Project cache directory is missing");

        if (!project->path().mkdir("cache")) {
            throw E_StoreException("Unable to create the project cache directory");
        }
    }

    if (!project->path().exists("tmp")) {
        qWarning("Project temporary directory is missing");

        if (!project->path().mkdir("tmp")) {
            throw E_StoreException("Unable to create the project temporary directory");
        }
    }

    if (!project->path().exists("targets")) {
        throw E_StoreException("Project targets directory is missing");
    }

    if (!project->path().exists("README.txt")) {
        qWarning("Project readme file is missing");

        QFile readmeFile(project->path().absoluteFilePath("README.txt"));
        if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project readme file");
        }
        readmeFile.write("TODO\n");
        readmeFile.close();
    }

    if (!project->path().exists("LICENSE.txt")) {
        qWarning("Project license file is missing");

        QFile licenseFile(project->path().absoluteFilePath("LICENSE.txt"));
        if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
            throw E_StoreException("Unable to create the project license file");
        }
        licenseFile.write("TODO\n");
        licenseFile.close();
    }

    if (!project->path().exists("INFO.txt")) {
        qWarning("Project information file is missing");

        QFile infoFile(project->path().absoluteFilePath("INFO.txt"));
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

    QDir path(project->path());

    if (!path.exists()) {
        throw E_StoreException(fromQString(tr("Project directory doesn't exists")));
    }

    // items
    if (path.exists("items")) {
        // @todo delete all its content

        path.rmdir("items");
    }

    // cache
    if (path.exists("cache")) {
        // @todo delete all its content

        path.rmdir("cache");
    }

    // tmp
    if (path.exists("tmp")) {
        // @todo delete all its content

        path.rmdir("tmp");
    }

    // targets
    if (path.exists("targets")) {
        // @todo delete all its content

        path.rmdir("targets");
    }

    // README
    if (path.exists("README.txt")) {
        path.remove("README.txt");
    }

    // LICENSE
    if (path.exists("LICENSE.txt")) {
        path.remove("LICENSE.txt");
    }

    // INFO
    if (path.exists("INFO.txt")) {
        path.remove("INFO.txt");
    }

    // project directory himself if empty
    if (path.isEmpty() && path.cdUp()) {
        path.rmdir(project->name());
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

StoreItem *Store::addItem(const QString &originalFileName)
{
    StoreItem *item = nullptr;

    return item;
}

const StoreItem *Store::addItem(const QString &originalFileName) const
{
    StoreItem *item = nullptr;

    return item;
}

bool Store::removeItem(const QUuid &uuid)
{
    return false;
}

bool Store::removeItem(StoreItem *item)
{
    return false;
}

bool Store::deleteItem(const QUuid &uuid)
{
    return false;
}

bool Store::deleteItem(StoreItem *item)
{
    return false;
}

void Store::purgeTrash(Project *project)
{

}

QList<StoreItem *> Store::removedItems(Project *project)
{
    QList<StoreItem*> items;

    return items;
}

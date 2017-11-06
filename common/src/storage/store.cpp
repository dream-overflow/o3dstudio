/**
 * @brief Common storage manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
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

void Store::initProject(Project *project)
{
    if (project->path().exists()) {
        throw StoreException(tr("Project path exists"));
    }

    if (!Application::instance()->workspaceManager().defaultProjectsPath().mkdir(project->name())) {
        throw StoreException("Unable to create the project directory");
    }

    /*if (!project->path().mkdir(".")) {
        throw StoreException("Unable to create the project directory");
    }*/

    if (!project->path().mkdir("items")) {
        throw StoreException("Unable to create the project items directory");
    }

    if (!project->path().mkdir("cache")) {
        throw StoreException("Unable to create the project cache directory");
    }

    if (!project->path().mkdir("tmp")) {
        throw StoreException("Unable to create the project temporary directory");
    }

    if (!project->path().mkdir("targets")) {
        throw StoreException("Unable to create the project targets path");
    }

    QFile readmeFile(project->path().absoluteFilePath("README.txt"));
    if (!readmeFile.open(QFile::WriteOnly | QFile::Text)) {
        throw StoreException("Unable to create the project readme file");
    }
    readmeFile.write("TODO\n");
    readmeFile.close();

    QFile licenseFile(project->path().absoluteFilePath("LICENSE.txt"));
    if (!licenseFile.open(QFile::WriteOnly | QFile::Text)) {
        throw StoreException("Unable to create the project licnse file");
    }
    licenseFile.write("TODO\n");
    licenseFile.close();

    QFile infoFile(project->path().absoluteFilePath("INFO.txt"));
    if (!infoFile.open(QFile::WriteOnly | QFile::Text)) {
        throw StoreException("Unable to create the project information file");
    }
    infoFile.write("TODO\n");
    infoFile.close();
}

void Store::loadProject(Project *project)
{
    // @todo check project structure, integrity
}

void Store::deleteProject(Project *project)
{
    if (project->path().exists()) {
        throw StoreException(tr("Project path exists"));
    }

    project->path().rmdir(".");
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

StoreException::StoreException(const QString &message) :
    BaseException(message)
{

}

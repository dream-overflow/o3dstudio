/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QDataStream>
#include <QtCore/QUuid>

#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/projectinfo.h"
#include "o3d/studio/common/workspace/projectfile.h"

#include "o3d/studio/common/objectref.h"

#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/asset.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/component/component.h"
#include "o3d/studio/common/component/dummyhub.h"

using namespace o3d::studio::common;

const char ProjectFile::PROJECT_MAGIC[] = "O3DSTUDIO";


ProjectFile::ProjectFile(Project *project, ProjectFile::ProjectVersion version) :
    m_project(project),
    m_version(version)
{
    O3D_ASSERT(project != nullptr);
}

ProjectFile::~ProjectFile()
{

}

const String &ProjectFile::name() const
{
    return m_project->name();
}

o3d::String ProjectFile::filename() const
{
    return m_project->path().makeFullFileName("project.o3dstudio");
}

const o3d::Dir &ProjectFile::path() const
{
    return m_project->path();
}

void ProjectFile::create()
{
    save();
}

void ProjectFile::load()
{
    QFile file(toQString(filename()));
    if (!file.exists()) {
        throw E_ProjectException(fromQString(tr("Missing project file for %1").arg(m_project->name())));
    }

    file.open(QFile::ReadOnly);

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw E_ProjectException(fromQString(tr("Project file streaming output not ok")));
    }

    // header
    char lmagic[9] = {0};

    const int magicLen = sizeof(PROJECT_MAGIC) - 1;

    int size = stream.readRawData(lmagic, magicLen);
    if ((size != magicLen) || (memcmp(lmagic, PROJECT_MAGIC, magicLen) != 0)) {
        throw E_ProjectException(fromQString(tr("Invalid project file format")));
    }

    QUuid uuid;
    QString name;

    stream >> uuid
           >> name;

    m_project->setRef(ObjectRef::buildRef(m_project->workspace(), uuid));

    if (m_project->name() != name) {
        throw E_ProjectException(fromQString(tr("Invalid project name")));
    }

    // id generator
    stream >> m_project->m_nextId;

    // global info
    stream >> *m_project->m_info;

    // read assets
    QString typeName;
    qint32 num = 0;
    stream >> num;

    Asset *asset = nullptr;
    for (qint32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        asset = new Asset("", m_project);
        asset->setRef(ObjectRef::buildRef(m_project, Application::instance()->types().typeRef(typeName), uuid));
        stream >> *asset;

        asset->setProject(m_project);
        m_project->m_assets.insert(asset->ref().light().id(), asset);
    }

    // read hubs
    stream >> num;

    ComponentRegistry &components = Application::instance()->components();
    Component *component = nullptr;

    Hub *hub = nullptr;
    for (qint32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        // @todo ComponentFactory
        component = components.componentByTarget(typeName);
        if (component) {
            hub = component->buildHub("", m_project, m_project);
            hub->setRef(ObjectRef::buildRef(m_project, component->targetTypeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;
        } else {
            hub = new DummyHub("", m_project);
            hub->setRef(ObjectRef::buildRef(m_project, hub->typeRef(), uuid));
            hub->setProject(m_project);

            stream >> *hub;

            // and pass the extra data
            // stream.skipRawData(hubSize...) // @todo
        }

        m_project->m_hubs.insert(hub->ref().light().id(), hub);
    }

    // read fragments
    stream >> num;

    Fragment *fragment = nullptr;
    for (qint32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        fragment = new Fragment("", m_project);
        fragment->setRef(ObjectRef::buildRef(m_project, Application::instance()->types().typeRef(typeName), uuid));
        stream >> *fragment;

        fragment->setProject(m_project);
        m_project->m_fragments.insert(fragment->ref().light().id(), fragment);
    }

    file.close();
}

void ProjectFile::save()
{
    QFile file(m_project->path().absoluteFilePath("project.o3dstudio"));
    file.open(QFile::WriteOnly | QFile::Truncate);

    QDataStream stream(&file);

    if (stream.status() != QDataStream::Ok) {
        throw E_ProjectException(fromQString(tr("Project file streaming output not ok")));
    }

    // header
    const int magicLen = sizeof(PROJECT_MAGIC) - 1;
    stream.writeRawData(PROJECT_MAGIC, magicLen);
    stream << m_project->ref().uuid()
           << m_project->name();

    // id generator
    stream << m_project->m_nextId;

    // global info, update modification date to now
    m_project->m_info->modificationDate() = QDateTime::currentDateTime();

    stream << *m_project->m_info;

    // @todo for now all in the project file,
    // but later could be one file per asset, and how if there is more 10^4 hubs ?
    // could be interseting to split per chunks of N hubs per file

    // save assets
    qint32 num = m_project->m_assets.size();
    stream << num;

    Asset *asset = nullptr;
    foreach (asset, m_project->m_assets) {
        stream << asset->ref().uuid()
               << toQString(asset->ref().strong().typeName())
               << *asset;
    }

    // save hubs recursively
    num = m_project->m_hubs.size();
    stream << num;

    Hub *hub = nullptr;
    foreach (hub, m_project->m_hubs) {
        stream << hub->ref().uuid()
               << toQString(hub->ref().strong().typeName())
               << *hub;
    }

    // save fragments
    num = m_project->m_fragments.size();
    stream << num;

    Fragment *fragment = nullptr;
    foreach (fragment, m_project->m_fragments) {
        stream << fragment->ref().uuid()
               << toQString(fragment->ref().strong().typeName())
               << *fragment;
    }

    file.close();
}

/**
 * @brief Common project object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <o3d/core/fileinstream.h>
#include <o3d/core/fileoutstream.h>

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

const o3d::String &ProjectFile::name() const
{
    return m_project->name();
}

o3d::String ProjectFile::filename() const
{
    return m_project->path().makeFullFileName("project.o3dstudio");
}

const o3d::BaseDir &ProjectFile::path() const
{
    return m_project->path();
}

void ProjectFile::create()
{
    save();

    // now add root hub to project (other case is during loading)
    m_project->rootHub()->create();
}

void ProjectFile::load()
{
    FileInStream stream(filename());

    // header
    Char lmagic[9] = {0};

    const int magicLen = sizeof(PROJECT_MAGIC) - 1;

    int size = stream.read(lmagic, magicLen);
    if ((size != magicLen) || (memcmp(lmagic, PROJECT_MAGIC, magicLen) != 0)) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Invalid project file format"))));
    }

    Uuid uuid;
    String name;

    stream >> uuid
           >> name;

    m_project->setRef(ObjectRef::buildRef(m_project->workspace(), uuid));

    // project id is now valid
    m_project->rootHub()->setRef(ObjectRef::buildRef(m_project, m_project->rootHub()->typeRef()));

    if (m_project->name() != name) {
        O3D_ERROR(E_ProjectException(fromQString(tr("Invalid project name"))));
    }

    // id generator
    stream >> m_project->m_nextId;

    // global info
    stream >> *m_project->m_info;

    // read hubs
    m_project->rootHub()->deserializeContent(stream);

    // read fragments
    String typeName;
    Int32 num = 0;

    stream >> num;

    Fragment *fragment = nullptr;
    for (Int32 i = 0; i < num; ++i) {
        stream >> uuid
               >> typeName;

        fragment = new Fragment("", m_project);
        fragment->setRef(ObjectRef::buildRef(m_project, Application::instance()->types().typeRef(typeName), uuid));
        stream >> *fragment;

        fragment->setProject(m_project);

        m_project->m_fragments[fragment->ref().light().id()] = fragment;
        m_project->addEntity(fragment);
    }

    stream.close();
}

void ProjectFile::save()
{
    FileOutStream stream(m_project->path().makeFullFileName("project.o3dstudio"));

    // header
    const int magicLen = sizeof(PROJECT_MAGIC) - 1;
    stream.write(PROJECT_MAGIC, magicLen);
    stream << m_project->ref().uuid()
           << m_project->name();

    // id generator
    stream << m_project->m_nextId;

    // global info, update modification date to now
    m_project->m_info->modificationDate() = DateTime(True);

    stream << *m_project->m_info;

    UInt32 num;

    // save hubs recursively, and in order
    m_project->rootHub()->serializeContent(stream);

    // save fragments
    num = m_project->m_fragments.size();
    stream << num;

    const Fragment *fragment = nullptr;
    for (auto cit = m_project->m_fragments.cbegin(); cit != m_project->m_fragments.cend(); ++cit) {
        fragment = cit->second;

        stream << fragment->ref().uuid()
               << fragment->ref().strong().typeName()
               << *fragment;
    }

    stream.close();
}

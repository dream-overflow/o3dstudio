/**
 * @brief Common importer registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#include "o3d/studio/common/importer/importerregistry.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/importer/importer.h"

using namespace o3d::studio::common;

ImporterRegistry::ImporterRegistry(BaseObject *parent) :
    BaseObject(parent)
{

}

ImporterRegistry::~ImporterRegistry()
{
    TypeRegistry &types = Application::instance()->types();

    m_importersById.clear();

    for (auto it = m_importers.begin(); it != m_importers.end(); ++it) {
        Importer *importer = it->second;

        types.unregisterType(importer->typeRef());

        delete importer;
    }
}

void ImporterRegistry::initialize()
{

}

void ImporterRegistry::registerImporter(Importer *importer)
{
    if (importer) {
        auto it = m_importers.find(importer->name());
        if (it != m_importers.end()) {
            O3D_ERROR(E_ImporterException(fromQString(tr("Importer {0} already registred").arg(toQString(importer->name())))));
        }

        // register the importer type
        TypeRegistry &types = Application::instance()->types();
        if (!types.registerType(IMPORTER_TYPE_ID, importer->name())) {
            O3D_ERROR(E_ImporterException(fromQString(tr("Importer type {0} cannot by registered").arg(toQString(importer->name())))));
        }

        importer->setTypeRef(types.typeRef(importer->name()));

        m_importers[importer->name()] = importer;
        m_importersById[importer->typeRef().id()] = importer;

        // importer->setup();

        // signal for importer interested, the UI
        onImporterRegistered(importer->name());
    }
}

o3d::Bool ImporterRegistry::unregisterImporter(const String &name)
{
    auto it = m_importers.find(name);
    if (it != m_importers.end()) {
        Importer *importer = it->second;
        m_importers.erase(it);

        auto it2 = m_importersById.find(importer->typeRef().id());
        if (it2 != m_importersById.end()) {
            m_importersById.erase(it2);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(importer->typeRef());

        onImporterUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool ImporterRegistry::unregisterImporter(UInt64 id)
{
    auto it2 = m_importersById.find(id);
    if (it2 != m_importersById.end()) {
        Importer *importer = it2->second;
        String name = importer->name();

        m_importersById.erase(it2);

        auto it = m_importers.find(importer->name());
        if (it != m_importers.end()) {
            m_importers.erase(it);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(importer->typeRef());

        delete importer;

        onImporterUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool ImporterRegistry::unregisterImporter(const TypeRef &ref)
{
    return unregisterImporter(ref.id());
}

Importer *ImporterRegistry::importer(const String &name)
{
    auto it = m_importers.find(name);
    if (it != m_importers.end()) {
        return it->second;
    }

    return nullptr;
}

const Importer *ImporterRegistry::importer(const String &name) const
{
    auto cit = m_importers.find(name);
    if (cit != m_importers.cend()) {
        return cit->second;
    }

    return nullptr;
}

Importer *ImporterRegistry::importer(const TypeRef &ref)
{
    auto it = m_importersById.find(ref.id());
    if (it != m_importersById.end()) {
        return it->second;
    }

    return nullptr;
}

const Importer *ImporterRegistry::importer(const TypeRef &ref) const
{
    auto cit = m_importersById.find(ref.id());
    if (cit != m_importersById.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<const Importer *> ImporterRegistry::importerList() const
{
    std::list<const Importer*> results;

    for (auto cit = m_importers.begin(); cit != m_importers.end(); ++cit) {
        results.push_back(cit->second);
    }

    return results;
}

/**
 * @brief Common builder registry
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-30
 * @details
 */

#include "o3d/studio/common/builder/builderregistry.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/typeregistry.h"

#include "o3d/studio/common/builder/builder.h"

#include <o3d/core/debug.h>

using namespace o3d::studio::common;

BuilderRegistry::BuilderRegistry(BaseObject *parent) :
    BaseObject(parent)
{

}

BuilderRegistry::~BuilderRegistry()
{
    TypeRegistry &types = Application::instance()->types();

    m_buildersById.clear();

    for (auto it = m_builders.begin(); it != m_builders.end(); ++it) {
        Builder *builder = it->second;

        types.unregisterType(builder->typeRef());

        delete builder;
    }
}

void BuilderRegistry::initialize()
{

}

void BuilderRegistry::registerBuilder(Builder *builder)
{
    if (builder) {
        auto it = m_builders.find(builder->name());
        if (it != m_builders.end()) {
            O3D_ERROR(E_BuilderException(fromQString(tr("Builder {0} already registred").arg(toQString(builder->name())))));
        }

        // register the builder type
        TypeRegistry &types = Application::instance()->types();
        if (!types.registerType(IMPORTER_TYPE_ID, builder->name())) {
            O3D_ERROR(E_BuilderException(fromQString(tr("Builder type {0} cannot by registered").arg(toQString(builder->name())))));
        }

        builder->setTypeRef(types.typeRef(builder->name()));

        m_builders[builder->name()] = builder;
        m_buildersById[builder->typeRef().id()] = builder;

        // builder->setup();

        // signal for builder interested, the UI
        onBuilderRegistered(builder->name());
    }
}

o3d::Bool BuilderRegistry::unregisterBuilder(const String &name)
{
    auto it = m_builders.find(name);
    if (it != m_builders.end()) {
        Builder *builder = it->second;
        m_builders.erase(it);

        auto it2 = m_buildersById.find(builder->typeRef().id());
        if (it2 != m_buildersById.end()) {
            m_buildersById.erase(it2);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(builder->typeRef());

        onBuilderUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool BuilderRegistry::unregisterBuilder(UInt64 id)
{
    auto it2 = m_buildersById.find(id);
    if (it2 != m_buildersById.end()) {
        Builder *builder = it2->second;
        String name = builder->name();

        m_buildersById.erase(it2);

        auto it = m_builders.find(builder->name());
        if (it != m_builders.end()) {
            m_builders.erase(it);
        }

        TypeRegistry &types = Application::instance()->types();
        types.unregisterType(builder->typeRef());

        delete builder;

        onBuilderUnregistered(name);

        return True;
    }

    return False;
}

o3d::Bool BuilderRegistry::unregisterBuilder(const TypeRef &ref)
{
    return unregisterBuilder(ref.id());
}

Builder *BuilderRegistry::builder(const String &name)
{
    auto it = m_builders.find(name);
    if (it != m_builders.end()) {
        return it->second;
    }

    return nullptr;
}

const Builder *BuilderRegistry::builder(const String &name) const
{
    auto cit = m_builders.find(name);
    if (cit != m_builders.cend()) {
        return cit->second;
    }

    return nullptr;
}

Builder *BuilderRegistry::builder(const TypeRef &ref)
{
    auto it = m_buildersById.find(ref.id());
    if (it != m_buildersById.end()) {
        return it->second;
    }

    return nullptr;
}

const Builder *BuilderRegistry::builder(const TypeRef &ref) const
{
    auto cit = m_buildersById.find(ref.id());
    if (cit != m_buildersById.cend()) {
        return cit->second;
    }

    return nullptr;
}

std::list<const Builder *> BuilderRegistry::builderList() const
{
    std::list<const Builder*> results;

    for (auto cit = m_builders.begin(); cit != m_builders.end(); ++cit) {
        results.push_back(cit->second);
    }

    return results;
}

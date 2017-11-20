/**
 * @brief Common module manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"
#include "o3d/studio/common/modulemanager.h"
#include "o3d/studio/common/dynamicmodule.h"

#include <o3d/core/debug.h>
#include <o3d/core/dynamiclibrary.h>
#include <o3d/core/diskdir.h>

#include <algorithm>

using namespace o3d::studio::common;

ModuleManager* ModuleManager::m_instance = nullptr;

ModuleManager::ModuleManager()
{
    // avoid recursive call when ctor call himself
    m_instance = (ModuleManager*)this;

    // some defaults
    Settings &settings = Application::instance()->settings();

    if (!settings.has("o3s::plugin::path")) {
        settings.set("o3s::plugin::path", PLUGIN_PATH);
    }

    if (!settings.has("o3s::plugin::exts")) {
        settings.set("o3s::plugin::exts", MODULE_EXT);
    }

    m_path = fromQString(settings.get("o3s::plugin::path").toString());
    m_filters = fromQString(settings.get("o3s::plugin::exts").toString());
}

ModuleManager::~ModuleManager()
{
    // @todo unload resting modules
}

// Singleton instantiation
ModuleManager* ModuleManager::instance()
{
    if (!m_instance) {
        m_instance = new ModuleManager();
    }

    return m_instance;
}

// Singleton destruction
void ModuleManager::destroy()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

o3d::T_StringList ModuleManager::moduleList() const
{
    T_StringList values;

    for (auto it = m_foundModules.begin(); it != m_foundModules.end(); ++it) {
        values.push_back(it->first);
    }

    return values;
}

void ModuleManager::setPluginsPath(const String &path)
{
    m_path = path;
}

void ModuleManager::setPluginsFilters(const String &filters)
{
    m_filters = filters;
}

void ModuleManager::searchModules()
{
    // lookup for plugins directory
    DiskDir dir(Application::instance()->appDir() + '/'  + m_path);
    if (!dir.isAbsolute()) {
        dir.makeAbsolute();
    }

    Int32 pos = 0;
    T_StringList plugins = dir.findFiles(m_filters, o3d::FILE_FILE);
    for (String plugin : plugins) {
        String name(plugin);

        if (name.startsWith("lib")) {
            name.remove(0, 3);
        }

        pos = name.reverseFind('.');
        if (pos > 0) {
            name.truncate(pos);
            m_foundModules[name] = dir.getPathName() + '/' + plugin;
        }
    }
}

o3d::Bool ModuleManager::hasModule(const String &name) const
{
    auto cit = m_foundModules.find(name);
    return cit != m_foundModules.cend();
}

Module *ModuleManager::module(const String &name) const
{
    auto cit = m_loadedModules.find(name);
    if (cit != m_loadedModules.cend()) {
        return cit->second;
    } else {
        return nullptr;
    }
}

Module *ModuleManager::load(const String &name)
{
    auto moduleEntry = m_foundModules.find(name);
    if (moduleEntry != m_foundModules.end()) {
        DynamicLibrary *library = nullptr;

        try {
            library = DynamicLibrary::load(moduleEntry->second);
        } catch (E_DynamicLibraryException &e) {
            return nullptr;
        }

        PluginFunction *fptr = (PluginFunction*)library->getFunctionPtr("o3dstudioPlugin");
        if (!fptr) {
            DynamicLibrary::unload(library);
            return nullptr;
        }

        Module *module = ((PluginFunction)fptr)(name, library);
        if (!module) {
            DynamicLibrary::unload(library);
            return nullptr;
        }

        m_loadedModules[name] = module;

        O3D_MESSAGE(String("Loaded module: " + name + " from " + library->getName()));

        return module;
    }

    return nullptr;
}

void ModuleManager::unloadAll()
{
    for (auto it = m_loadedModules.begin(); it != m_loadedModules.end(); ++it) {
        Module *module = it->second;
        DynamicLibrary *library = module->library();

        O3D_MESSAGE(String("Unload module: " + it->first + " from " + library->getName()));

        delete module;

        if (library) {
            DynamicLibrary::unload(library);
        }
    }

    m_loadedModules.clear();
}

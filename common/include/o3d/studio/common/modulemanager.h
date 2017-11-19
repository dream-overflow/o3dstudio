/**
 * @brief Common module manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_MODULEMANAGER_H
#define _O3DS_COMMON_MODULEMANAGER_H

#include <map>

#include <o3d/core/stringlist.h>

#include "global.h"
#include "module.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API ModuleManager
{
public:  // singleton

    //! Get the singleton instance.
    static ModuleManager* instance();

    //! Delete the singleton instance.
    static void destroy();

public:

    /**
     * @brief Return the list of found modules names.
     * @return
     */
    T_StringList moduleList() const;

    /**
     * @brief setPluginsPath
     * @param path
     */
    void setPluginsPath(const String &path);

    /**
     * @brief setPluginsFilters
     * @param filters extensions separated by |
     */
    void setPluginsFilters(const String &filters);

    /**
     * @brief Refresh the list of modules.
     */
    void searchModules();

    /**
     * @brief Has found a given module name
     * @param name
     * @return
     */
    Bool hasModule(const String &name) const;

    /**
     * @brief Get a previously loaded module.
     * @param name Name of the module
     * @return Module or nullptr.
     */
    Module* module(const String &name) const;

    /**
     * @brief load a dynamic module.
     * @param name Module name.
     * @return Instance of the module.
     */
    Module* load(const String &name);

    /**
     * @brief unload a previously dynamic module and delete the object.
     * @param module A valid object.
     */
    void unload(Module *module);

    /**
     * @brief Unload any loaded modules.
     */
    void unloadAll();

    /**
     * @brief loadLanguage
     * @param language
     */
    // void loadLanguage(const QString &language);

private:

    String m_path;
    String m_filters;   //! extensions separated by |

    std::map<String, String> m_foundModules;
    std::map<String, Module*> m_loadedModules;

private:  // singleton

    static ModuleManager* m_instance;

    //! Restricted default contructor
    ModuleManager();

    //! Restricted destructor
    ~ModuleManager();

    //! Non copyable
    ModuleManager(const ModuleManager&);

    //! Non assignable
    void operator=(const ModuleManager&);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MODULEMANAGER_H

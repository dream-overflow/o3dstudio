/**
 * @brief Common module manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_MODULEMANAGER_H
#define _O3DS_COMMON_MODULEMANAGER_H

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QLoggingCategory>

#include "module.h"

namespace o3d {
namespace studio {
namespace common {

Q_DECLARE_LOGGING_CATEGORY(moduleLogger)

class ModuleManager
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
    QStringList getModuleList() const;

    /**
     * @brief setPluginsPath
     * @param path
     */
    void setPluginsPath(const QString &path);

    /**
     * @brief setPluginsFilters
     * @param filters
     */
    void setPluginsFilters(const QStringList &filters);

    /**
     * @brief Refresh the list of modules.
     */
    void searchModules();

    /**
     * @brief Has found a given module name
     * @param name
     * @return
     */
    bool hasModule(const QString &name) const;

    /**
     * @brief Get a previously loaded module.
     * @param name Name of the module
     * @return Module or nullptr.
     */
    Module* getModule(const QString &name) const;

    /**
     * @brief load a dynamic module.
     * @param name Module name.
     * @return Instance of the module.
     */
    Module* load(const QString &name);

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

    QString m_path;
    QStringList m_filters;

    QMap<QString, QString> m_foundModules;
    QMap<QString, Module*> m_loadedModules;

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

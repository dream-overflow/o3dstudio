/**
 * @brief Common module manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#include<QtCore/QLibrary>
#include<QtCore/QDir>
#include<QtCore/QCoreApplication>
#include<QtCore/QUrl>
#include<QtCore/QStringList>

#include "common/application.h"
#include "common/settings.h"
#include "common/modulemanager.h"
#include "common/dynamicmodule.h"

using namespace o3d::studio::common;

Q_LOGGING_CATEGORY(o3d::studio::common::moduleLogger, "o3d::studio::common:module")

ModuleManager* ModuleManager::m_instance = nullptr;

ModuleManager::ModuleManager()
{
    // avoid recursive call when ctor call himself
    m_instance = (ModuleManager*)this;

    // some defaults
    Settings &settings = Application::instance()->settings();

    if (!settings.has("o3s::plugin::path")) {
        settings.set("o3s::plugin::path", QUrl::fromLocalFile(PLUGIN_PATH));
    }

    if (!settings.has("o3s::plugin::exts")) {
        settings.set("o3s::plugin::exts", QStringList(QString(MODULE_EXT)));
    }

    m_path = settings.get("o3s::plugin::path").toUrl().toLocalFile();
    m_filters = settings.get("o3s::plugin::exts").toStringList();
}

ModuleManager::~ModuleManager()
{
    // @todo unload resting modules
}

// Singleton instantiation
ModuleManager* ModuleManager::instance()
{
    if (!m_instance)
        m_instance = new ModuleManager();
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

QStringList ModuleManager::getModuleList() const
{
    return m_foundModules.keys();
}

void ModuleManager::setPluginsPath(const QString &path)
{
    m_path = path;
}

void ModuleManager::setPluginsFilters(const QStringList &filters)
{
    m_filters = filters;
}

void ModuleManager::searchModules()
{
    // lookup for plugins directory
    QDir dir(m_path);
    if (dir.isRelative()) {
        dir.setPath(QCoreApplication::applicationDirPath() + QDir::separator() + m_path);
    }

    dir.setNameFilters(m_filters);

    QStringList plugins = dir.entryList(QDir::Files, QDir::Name);
    QString plugin;
    foreach (plugin, plugins) {
        QString name(plugin);

        if (name.startsWith("lib")) {
            name.replace("lib", "");
        }

        name = name.split(".")[0];

        m_foundModules.insert(name, dir.path() + QDir::separator() + plugin);
    }
}

bool ModuleManager::hasModule(const QString &name) const
{
    auto cit = m_foundModules.find(name);
    return cit != m_foundModules.constEnd();
}

Module *ModuleManager::getModule(const QString &name) const
{
    auto cit = m_loadedModules.find(name);
    if (cit != m_loadedModules.constEnd()) {
        return cit.value();
    } else {
        return nullptr;
    }
}

Module *ModuleManager::load(const QString &name)
{
    auto moduleEntry = m_foundModules.find(name);
    if (moduleEntry != m_foundModules.end()) {
        QLibrary *library = new QLibrary(moduleEntry.value());

        if (!library->load()) {
            delete library;
            return nullptr;
        }

        QFunctionPointer fptr = library->resolve("o3dstudioPlugin");
        if (!fptr) {
            library->unload();
            delete library;

            return nullptr;
        }

        Module *module = ((PluginFunction)fptr)(name, library);
        if (!module) {
            library->unload();
            delete library;

            return nullptr;
        }

        m_loadedModules.insert(name, module);

        qCInfo(moduleLogger) << "Loaded module :" << name << "from" << library->fileName();
        return module;
    }

    return nullptr;
}

void ModuleManager::unloadAll()
{
    for (auto it = m_loadedModules.begin(); it != m_loadedModules.end(); ++it) {
        Module *module = it.value();
        QLibrary *library = module->library();

        qCInfo(moduleLogger) << "Unload module :" << it.key() << "from" << library->fileName();
        delete module;

        if (library) {
            if (library->isLoaded()) {
                library->unload();
                delete library;
            }
        }
    }

    m_loadedModules.clear();
}
/*
void ModuleManager::loadLanguage(const QString &language)
{
    if (m_currentLanguage != language) {
        QLocale locale = QLocale::system();
        QString languageCode = language;

        if (language != "default") {
            locale = QLocale(language);
        } else {
            languageCode = locale.name().split("_").at(0);
        }

        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        switchTranslator(m_translator, QString("o3smain_%1.qm").arg(languageCode));
        switchTranslator(m_translator, QString("o3scommon_%1.qm").arg(languageCode));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(languageCode));

        // @todo and logger status bar message at certain levels
        statusBar()->showMessage(tr("Current language changed to %1").arg(languageName));

        m_currentLanguage = language;
    }
}
*/

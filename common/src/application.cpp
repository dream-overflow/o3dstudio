/**
 * @brief Common application singleton
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/modulemanager.h"
#include "o3d/studio/common/typeregistry.h"
#include "o3d/studio/common/messenger.h"

#include "o3d/studio/common/component/componentregistry.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/ui/uicontroller.h"
#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/storage/store.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/importer/importerregistry.h"
#include "o3d/studio/common/builder/builderregistry.h"

#include <o3d/core/application.h>
#include <o3d/core/filemanager.h>
#include <o3d/core/gl.h>

using namespace o3d::studio::common;

Application* Application::m_instance = nullptr;

Application::Application() :
    m_started(False)
{
    // qRegisterMetaType<LightRef>("LightRef");

    // avoid recursive call when ctor call himself
    m_instance = (Application*)this;

    // get working directory as path
    m_workingDir = FileManager::instance()->getWorkingDirectory();
    m_appDir = o3d::Application::getAppPath();

    // instanciate, take care of ordering
    m_typeRegistry = new TypeRegistry();
    m_messenger = new Messenger();
    m_importerRegistry = new ImporterRegistry();
    m_builderRegistry = new BuilderRegistry();
    m_selection = new Selection();
    m_commandManager = new CommandManager();
    m_store = new Store();
    m_workspaceManager = new WorkspaceManager();
    m_componentRegistry = new ComponentRegistry();
    m_ui = new UiController();

    // initialize
    m_commandManager->initialize();
    m_componentRegistry->initialize();
    m_importerRegistry->initialize();
    m_builderRegistry->initialize();
}

Application::~Application()
{
    // take care of ordering
    delete m_ui;
    delete m_selection;
    delete m_workspaceManager;
    delete m_commandManager;
    delete m_store;
    delete m_componentRegistry;
    delete m_importerRegistry;
    delete m_builderRegistry;
    delete m_messenger;
    delete m_typeRegistry;
}

// Singleton instantiation
Application* Application::instance()
{
    if (!m_instance) {
        m_instance = new Application();
    }

    return m_instance;
}

o3d::Bool Application::hasInstance()
{
    return m_instance != nullptr;
}

// Singleton destruction
void Application::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

const o3d::String &Application::appDir() const
{
    return m_appDir;
}

const o3d::String& Application::workingDir() const
{
    return m_workingDir;
}

Settings &Application::settings()
{
    return m_settings;
}

const Settings &Application::settings() const
{
    return m_settings;
}

WorkspaceManager &Application::workspaces()
{
    return *m_workspaceManager;
}

const WorkspaceManager &Application::workspaces() const
{
    return *m_workspaceManager;
}

UiController &Application::ui()
{
    return *m_ui;
}

const UiController &Application::ui() const
{
    return *m_ui;
}

CommandManager &Application::command()
{
    return *m_commandManager;
}

const CommandManager &Application::command() const
{
    return *m_commandManager;
}

Store &Application::store()
{
    return *m_store;
}

const Store &Application::store() const
{
    return *m_store;
}

Selection &Application::selection()
{
    return *m_selection;
}

const Selection &Application::selection() const
{
    return *m_selection;
}

Messenger &Application::messenger()
{
    return *m_messenger;
}

const Messenger &Application::messenger() const
{
    return *m_messenger;
}

ComponentRegistry &Application::components()
{
    return *m_componentRegistry;
}

const ComponentRegistry &Application::components() const
{
    return *m_componentRegistry;
}

TypeRegistry &Application::types()
{
    return *m_typeRegistry;
}

const TypeRegistry &Application::types() const
{
    return *m_typeRegistry;
}

ImporterRegistry &Application::importers()
{
    return *m_importerRegistry;
}

const ImporterRegistry &Application::importers() const
{
    return *m_importerRegistry;
}

BuilderRegistry &Application::builders()
{
    return *m_builderRegistry;
}

const BuilderRegistry &Application::builders() const
{
    return *m_builderRegistry;
}

o3d::Bool Application::start()
{
    m_settings.loadAll();

    ModuleManager::instance()->setPluginsPath(fromQString(m_settings.get("o3s::plugin::path").toString()));
    ModuleManager::instance()->setPluginsFilters(fromQString(m_settings.get("o3s::plugin::exts").toString()));
    ModuleManager::instance()->searchModules();

    m_commandManager->begin();

    // auto load all found modules
    // @todo need an auto load list
    T_StringList modules = ModuleManager::instance()->moduleList();
    for (String name : modules) {
        Module *module = ModuleManager::instance()->load(name);
        if (module != nullptr) {
            try {
                module->start();
            } catch (E_BaseException &e) {
                // continue with next
            }
        }
    }

//    Module *o3sdummy = ModuleManager::instance()->load("o3sdummy");
//    if (o3sdummy != nullptr) {
//        o3sdummy->start();
//    }

    m_started = True;
    return True;
}

o3d::Bool Application::stop()
{
    m_settings.saveAll();

    m_commandManager->finish();

    while (m_commandManager->hasPendingCommands() || m_commandManager->hasRunningCommands()) {
        System::waitMs(20);
    }

    ModuleManager::instance()->unloadAll();
    ModuleManager::destroy();

    m_started = False;
    return True;
}

void switchTranslator(QTranslator& translator, const QString& filename, QString path = QString())
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if (path.isEmpty()) {
        path = toQString(Application::instance()->appDir()) + '/' + QString(LANGUAGES_PATH);
    }

    if (translator.load(filename, path, QString(), QString())) {
        qApp->installTranslator(&translator);
    }
}

void Application::loadLanguage(const String &language)
{
    if (m_currentLanguage != language) {
        QLocale locale = QLocale::system();
        QString languageCode = toQString(language);

        if (language != "default") {
            locale = QLocale(toQString(language));
        } else {
            languageCode = locale.name().split("_").at(0);
        }

        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        switchTranslator(m_translator, QString("o3scommon_%1.qm").arg(languageCode));
        switchTranslator(
                    m_translatorQt,
                    QString("qt_%1.qm").arg(languageCode),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath));

        m_currentLanguage = language;
    }
}

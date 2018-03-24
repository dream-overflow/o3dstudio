/**
 * @brief Common application singleton
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_APPLICATION_H
#define _O3DS_COMMON_APPLICATION_H

#include <QtCore/QTranslator>

#include "global.h"
#include "settings.h"

namespace o3d {
namespace studio {
namespace common {

class WorkspaceManager;
class UiController;
class CommandManager;
class Store;
class Selection;
class Messenger;
class ComponentRegistry;
class TypeRegistry;
class ImporterRegistry;
class BuilderRegistry;

class O3S_API Application
{
public:  // singleton

    //! Get the singleton instance.
    static Application* instance();

    //! Has a singleton instance.
    static Bool hasInstance();

    //! Delete the singleton instance.
    static void destroy();

public:

    //
    // Properties
    //

    const String& appDir() const;
    const String& workingDir() const;

    // loads a language by the given language shortcurt (e.g. de, en)
    void loadLanguage(const String& language);

    //
    // Startup
    //

    Bool start();
    Bool stop();
    Bool setupUi();

    //
    // Controllers
    //

    Settings& settings();
    const Settings& settings() const;

    WorkspaceManager& workspaces();
    const WorkspaceManager& workspaces() const;

    UiController& ui();
    const UiController& ui() const;

    CommandManager& command();
    const CommandManager& command() const;

    Store& store();
    const Store& store() const;

    Selection& selection();
    const Selection& selection() const;

    Messenger& messenger();
    const Messenger& messenger() const;

    ComponentRegistry& components();
    const ComponentRegistry& components() const;

    TypeRegistry& types();
    const TypeRegistry& types() const;

    ImporterRegistry& importers();
    const ImporterRegistry& importers() const;

    BuilderRegistry& builders();
    const BuilderRegistry& builders() const;

    //
    // Factories
    //

private:

    String m_appDir;             //!< Absolute application base directory
    String m_workingDir;         //!< Absolute application working directory

    Settings m_settings;          //!< Setting manager

    WorkspaceManager *m_workspaceManager;
    CommandManager *m_commandManager;
    UiController *m_ui;
    Store *m_store;
    Selection *m_selection;
    Messenger *m_messenger;
    ComponentRegistry *m_componentRegistry;
    TypeRegistry *m_typeRegistry;
    ImporterRegistry *m_importerRegistry;
    BuilderRegistry *m_builderRegistry;

    Bool m_started;               //!< Started application

    QTranslator m_translator;     //!< contains the translations for o3scommon
    QTranslator m_translatorQt;   //!< contains the translations for qt

    String m_currentLanguage;

private:  // singleton

    static Application* m_instance;

    //! Restricted default contructor
    Application();

    //! Restricted destructor
    ~Application();

    //! Non copyable
    Application(const Application&);

    //! Non assignable
    void operator=(const Application&);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_APPLICATION_H

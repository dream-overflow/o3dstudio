/**
 * @brief Common application singleton
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_APPLICATION_H
#define _O3DS_COMMON_APPLICATION_H

#include <QtCore/QString>
#include <QtCore/QTranslator>

#include "settings.h"

namespace o3d {
namespace studio {
namespace common {

class WorkspaceManager;

class Application
{
public:  // singleton

    //! Get the singleton instance.
    static Application* instance();

    //! Delete the singleton instance.
    static void destroy();

public:

    const QString& appDir() const;
    const QString& workingDir() const;

    Settings& settings();
    const Settings& settings() const;

    WorkspaceManager& workspaceManager();
    const WorkspaceManager& workspaceManager() const;

    bool start();
    bool stop();

    // loads a language by the given language shortcurt (e.g. de, en)
    void loadLanguage(const QString& language);

private:

    QString m_appDir;             //!< Absolute application base directory
    QString m_workingDir;         //!< Absolute application working directory

    Settings m_settings;          //!< Setting manager

    WorkspaceManager *m_workspaceManager;

    bool m_started{false};        //!< Started application

    QTranslator m_translator;     //!< contains the translations for o3scommon
    QTranslator m_translatorQt;   //!< contains the translations for qt

    QString m_currentLanguage;

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

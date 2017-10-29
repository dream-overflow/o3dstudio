/**
 * @brief Common command base
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_COMMAND_H
#define _O3DS_COMMON_COMMAND_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace o3d {
namespace studio {
namespace common {

enum CommandState {
    COMMAND_READY = 0,
    COMMAND_EXECUTED = 1,
    COMMAND_UNDONE = 2,
    COMMAND_DONE = 3
};

class Command
{
public:

    Command(const QMap<QString, QVariant> &options);
    virtual ~Command();

    /**
     * @brief Internal command name with full prefixes.
     */
    QString commandName() const;

    /**
     * @brief Last command state.
     */
    CommandState commandState() const;

    /**
     * @brief Command options map.
     */
    const QMap<QString, QVariant>& commandOptions() const;

    /**
     * @brief Return a translated verbose string of the command to display.
     */
    virtual QString commandLabel() const;

    /**
     * @brief First processing of the command.
     * @return true if success
     */
    virtual bool executeCommand() = 0;

    /**
     * @brief Undo the command after a previous execute or redo.
     * @return true if success
     */
    virtual bool undoCommand() = 0;

    /**
     * @brief Redo the command after a previous undo.
     * @return true if success
     */
    virtual bool redoCommand() = 0;

protected:

    //! command name
    QString m_commandName;

    //! last command state
    CommandState m_commandState;

    //! local commands options
    const QMap<QString, QVariant> &m_commandOptions;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMMAND_H

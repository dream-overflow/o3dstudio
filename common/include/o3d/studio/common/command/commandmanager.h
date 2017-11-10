/**
 * @brief Common global command manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_COMMANDMANAGER_H
#define _O3DS_COMMON_COMMANDMANAGER_H

#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QReadWriteLock>

#include "command.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API CommandManager : public QThread
{
    Q_OBJECT

public:

    CommandManager();
    virtual ~CommandManager();

    /**
     * @brief Add and execute or deferred execution of a new command.
     * @return true if success
     */
    void addCommand(Command *cmd);

    /**
     * @brief Undo the last command.
     * @return true if success
     */
    void undoLastCommand();

    /**
     * @brief Undo the last dones n commands.
     */
    void undoNCommands(int num);

    /**
     * @brief Redo the left command.
     * @return true if success
     */
    void redoLastCommand();

    /**
     * @brief Redo the last n undones commands.
     */
    void redoNCommands(int num);

    /**
     * @brief Internal command result.
     */
    void setCommandDone(Command *cmd);

    bool hasPendingCommands() const;
    bool hasRunningCommands() const;
    bool hasDoneCommands() const;
    bool hasUndoneCommands() const;

    QStringList undoableCommandList() const;
    QStringList redoableCommandList() const;

    //
    // Thread management
    //

    virtual void run() override;

    void begin();
    void finish();

signals:

    void commandDone(QString name, QString label, bool done);

protected:

    //! queue to commands to be executed, to be executable
    QQueue<Command*> m_todoCommandsQueue;

    //! queue to commands to be terminated, when execute, redo, undo many commands at once
    QQueue<Command*> m_waitingCommandsQueue;

    //! queue for done commands, to be undoable
    QQueue<Command*> m_doneCommandsQueue;

    //! queue for undone commands, to be redoable
    QQueue<Command*> m_undoneCommandsQueue;

    bool m_running;

    QReadWriteLock m_rwLock;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMMANDMANAGER_H

/**
 * @brief Common global command manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_COMMANDMANAGER_H
#define _O3DS_COMMON_COMMANDMANAGER_H

#include <stack>

#include <o3d/core/baseobject.h>
#include <o3d/core/thread.h>
#include <o3d/core/stringlist.h>

#include "command.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

using o3d::studio::common::LightRef;

class O3S_API CommandManager : public BaseObject, public Runnable
{
public:

    CommandManager(BaseObject *parent = nullptr);
    virtual ~CommandManager();

    /**
     * @brief Second step initialization.
     */
    void initialize();

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
    void undoNCommands(Int32 num);

    /**
     * @brief Redo the left command.
     * @return true if success
     */
    void redoLastCommand();

    /**
     * @brief Redo the last n undones commands.
     */
    void redoNCommands(Int32 num);

    Bool hasPendingCommands() const;
    Bool hasRunningCommands() const;
    Bool hasDoneCommands() const;
    Bool hasUndoneCommands() const;

    T_StringList undoableCommandList() const;
    T_StringList redoableCommandList() const;

    String nextToUndo() const;
    String nextToRedo() const;

    //
    // Thread management
    //

    virtual Int32 run(void*) override;

    void begin();
    void finish();

public /*slots*/:

    void onChangeCurrentWorkspace(const String& name);
    void onProjectRemoved(LightRef ref);

public /*signals*/:

    /**
     * @brief Update commands list (todo, undoable, redoable)
     */
    Signal<> commandUpdate{this};

    /**
     * @brief Command done, undone or redone
     */
    Signal<String /*name*/, String /*label*/, Bool /*done*/> commandDone{this};

protected:

    //! queue to commands to be executed, to be executable
    std::list<Command*> m_todoCommandsStack;

    //! queue to commands to be terminated, when execute, redo, undo many commands at once
    std::list<Command*> m_waitingCommandsStack;

    //! queue for done commands, to be undoable
    std::list<Command*> m_doneCommandsStack;

    //! queue for undone commands, to be redoable
    std::list<Command*> m_undoneCommandsStack;

    Bool m_running;

    Thread m_thread;
    ReadWriteLock m_rwLock;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_COMMANDMANAGER_H

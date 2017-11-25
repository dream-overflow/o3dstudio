/**
 * @brief Common command manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/application.h"
#include "o3d/studio/common/exception.h"
#include "o3d/studio/common/messenger.h"

#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"

using namespace o3d::studio::common;


CommandManager::CommandManager(BaseObject *parent) :
    BaseObject(parent),
    m_running(False),
    m_thread(this)
{
}

CommandManager::~CommandManager()
{
    m_thread.waitFinish();  // join the thread

    O3D_ASSERT(m_todoCommandsStack.empty());
    O3D_ASSERT(m_waitingCommandsStack.empty());

    for (Command *cmd : m_doneCommandsStack) {
        delete cmd;
    }
    m_doneCommandsStack.clear();

    for (Command *cmd : m_undoneCommandsStack) {
        delete cmd;
    }
    m_undoneCommandsStack.clear();
}

void CommandManager::initialize()
{
    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    workspaceManager->onWorkspaceActivated.connect(this, &CommandManager::onChangeCurrentWorkspace);

    onChangeCurrentWorkspace(workspaceManager->current()->name());

}

void CommandManager::onChangeCurrentWorkspace(const String &/*name*/)
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        workspace->onProjectRemoved.connect(this, &CommandManager::onProjectRemoved);
    }
}

void CommandManager::onProjectRemoved(LightRef ref)
{
    m_rwLock.lockWrite();

    // remove any command related to the project
    std::list<std::list<Command*>::iterator> eraseItList;
    Command *cmd = nullptr;

    // done command stack
    for (auto it = m_doneCommandsStack.begin(); it != m_doneCommandsStack.end(); ++it) {
        cmd = *it;

        if (cmd->targetRef().projectId() == ref.projectId()) {
            delete cmd;
            eraseItList.push_back(it);
        }
    }

    for (auto it = eraseItList.begin(); it != eraseItList.end(); ++it) {
        m_doneCommandsStack.erase(*it);
    }

    eraseItList.clear();

    // todo commands stack
    for (auto it = m_todoCommandsStack.begin(); it != m_todoCommandsStack.end(); ++it) {
        cmd = *it;

        if (cmd->targetRef().projectId() == ref.projectId()) {
            delete cmd;
            eraseItList.push_back(it);
        }
    }

    for (auto it = eraseItList.begin(); it != eraseItList.end(); ++it) {
        m_todoCommandsStack.erase(*it);
    }

    eraseItList.clear();

    // done commands stack
    for (auto it = m_doneCommandsStack.begin(); it != m_doneCommandsStack.end(); ++it) {
        cmd = *it;

        if (cmd->targetRef().projectId() == ref.projectId()) {
            delete cmd;
            eraseItList.push_back(it);
        }
    }

    for (auto it = eraseItList.begin(); it != eraseItList.end(); ++it) {
        m_doneCommandsStack.erase(*it);
    }

    eraseItList.clear();

    // undone commands stack
    for (auto it = m_undoneCommandsStack.begin(); it != m_undoneCommandsStack.end(); ++it) {
        cmd = *it;

        if (cmd->targetRef().projectId() == ref.projectId()) {
            delete cmd;
            eraseItList.push_back(it);
        }
    }

    for (auto it = eraseItList.begin(); it != eraseItList.end(); ++it) {
        m_undoneCommandsStack.erase(*it);
    }

    eraseItList.clear();

    m_rwLock.unlockWrite();
}

void CommandManager::addCommand(Command *cmd)
{
    Bool update = False;

    m_rwLock.lockWrite();
    m_todoCommandsStack.push_back(cmd);

    if (m_undoneCommandsStack.size()) {
        // clear redo history
        for (Command *cmd : m_undoneCommandsStack) {
            delete cmd;
        }

        m_undoneCommandsStack.clear();
        update = True;
    }

    m_rwLock.unlockWrite();

    if (update) {
        commandUpdate();
    }
}

void CommandManager::undoLastCommand()
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_doneCommandsStack.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastCmd = m_doneCommandsStack.back();
    m_doneCommandsStack.pop_back();

    // @todo or todoCommandStack ?
    m_waitingCommandsStack.push_back(lastCmd);

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::undoNCommands(Int32 num)
{
    Int32 size = 0;

    m_rwLock.lockRead();
    size = m_doneCommandsStack.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastCmd = nullptr;
    std::list<Command*> cmdList;
    for (int i = 0; i < std::min(size, num); ++i) {
        // pop back last n command
        lastCmd = m_doneCommandsStack.back();
        m_doneCommandsStack.pop_back();

        cmdList.push_front(lastCmd);
    }

    for (Command *cmd : cmdList) {
        // inject into waiting list @todo or todoCommandStack ?
        m_waitingCommandsStack.push_back(cmd);
    }

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::redoLastCommand()
{
    Int32 size = 0;

    m_rwLock.lockRead();
    size = m_undoneCommandsStack.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastUndone = m_undoneCommandsStack.back();
    m_undoneCommandsStack.pop_back();

    // @todo or todoCommandStack ?
    m_waitingCommandsStack.push_back(lastUndone);

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::redoNCommands(Int32 num)
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_undoneCommandsStack.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastUndone = nullptr;
    std::list<Command*> cmdList;
    for (int i = 0; i < std::min(size, num); ++i) {
        // pop back last n command
        lastUndone = m_undoneCommandsStack.back();
        m_undoneCommandsStack.pop_back();

        cmdList.push_front(lastUndone);
    }

    for (Command *cmd : cmdList) {
        // inject into waiting list @todo or todoCommandStack ?
        m_waitingCommandsStack.push_back(cmd);
    }

    m_rwLock.unlockWrite();

    commandUpdate();
}

o3d::Bool CommandManager::hasPendingCommands() const
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_todoCommandsStack.size();
    m_rwLock.unlockRead();

    return size > 0;
}

o3d::Bool CommandManager::hasRunningCommands() const
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_waitingCommandsStack.size();
    m_rwLock.unlockRead();

    return size > 0;
}

o3d::Bool CommandManager::hasDoneCommands() const
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_doneCommandsStack.size();
    m_rwLock.unlockRead();

    return size > 0;
}

o3d::Bool CommandManager::hasUndoneCommands() const
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_undoneCommandsStack.size();
    m_rwLock.unlockRead();

    return size > 0;
}

o3d::T_StringList CommandManager::undoableCommandList() const
{
    T_StringList cmds;

    m_rwLock.lockRead();

    for (auto cit = m_doneCommandsStack.cbegin(); cit != m_doneCommandsStack.cend(); ++cit) {
        cmds.push_back((*cit)->commandLabel());
    }

    m_rwLock.unlockRead();

    return cmds;
}

o3d::T_StringList CommandManager::redoableCommandList() const
{
    T_StringList cmds;

    m_rwLock.lockRead();

    for (auto cit = m_undoneCommandsStack.cbegin(); cit != m_undoneCommandsStack.cend(); ++cit) {
        cmds.push_back((*cit)->commandLabel());
    }

    m_rwLock.unlockRead();

    return cmds;
}

o3d::String CommandManager::nextToUndo() const
{
    String label;

    m_rwLock.lockRead();

    if (m_doneCommandsStack.size()) {
        label = m_doneCommandsStack.back()->commandLabel();
    }

    m_rwLock.unlockRead();

    return label;
}

o3d::String CommandManager::nextToRedo() const
{
    String label;

    m_rwLock.lockRead();

    if (m_undoneCommandsStack.size()) {
        label = m_undoneCommandsStack.back()->commandLabel();
    }

    m_rwLock.unlockRead();

    return label;
}

o3d::Int32 CommandManager::run(void*)
{
    Bool run = True;
    Bool error = False;
    Messenger& messenger = Application::instance()->messenger();

    while (1) {
        Command *nextCmd = nullptr;

        m_rwLock.lockWrite();

        run = m_running | !m_waitingCommandsStack.empty() | !m_todoCommandsStack.empty();

        // first look in wait list, if empty look in todo list
        if (!m_waitingCommandsStack.empty()) {
            nextCmd = m_waitingCommandsStack.back();
            m_waitingCommandsStack.pop_back();
        } else if (!m_todoCommandsStack.empty()) {
            nextCmd = m_todoCommandsStack.back();
            m_todoCommandsStack.pop_back();
        }

        if (nextCmd != nullptr) {
            m_waitingCommandsStack.push_back(nextCmd);
        }

        m_rwLock.unlockWrite();

        if (nextCmd != nullptr) {
            error = false;

            try {
                if (nextCmd->commandState() == COMMAND_READY) {
                    // never executed, then execute
                    if (nextCmd->doCommand()) {
                        nextCmd->setDone();

                        commandDone(nextCmd->commandName(), nextCmd->commandLabel(), true);
                    }
                } else if (nextCmd->commandState() == COMMAND_REDONE) {
                    // previously redone, then undo
                    if (nextCmd->undoCommand()) {
                        nextCmd->setUndone();

                        commandDone(nextCmd->commandName(), nextCmd->commandLabel(), false);
                    }
                } else if (nextCmd->commandState() == COMMAND_DONE) {
                    // previously executed, then undo
                    if (nextCmd->undoCommand()) {
                        nextCmd->setUndone();

                        commandDone(nextCmd->commandName(), nextCmd->commandLabel(), false);
                    }
                } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                    // previously undone, then redo
                    if (nextCmd->redoCommand()) {
                        nextCmd->setReDone();

                        commandDone(nextCmd->commandName(), nextCmd->commandLabel(), true);
                    }
                }
            } catch (E_CommonException &e) {
                messenger.critical(e.getMsg());
                error = true;
            }

            m_rwLock.lockWrite();

            if (error) {
                nextCmd = nullptr;

                // cannot terminate current command batch, so delete them
                for (auto it = m_waitingCommandsStack.begin(); it != m_waitingCommandsStack.end(); ++it) {
                    delete *it;
                }

                m_waitingCommandsStack.clear();

                // and cannot performs next pending commands because of the possible coherency, so delete them to
                for (auto it = m_todoCommandsStack.begin(); it != m_todoCommandsStack.end(); ++it) {
                    delete *it;
                }

                m_todoCommandsStack.clear();
            } else {
                if (nextCmd->commandState() == COMMAND_READY) {
                    // error, may not arrives
                } else if (nextCmd->commandState() == COMMAND_REDONE) {
                    // in done queue, can be later undone
                    m_doneCommandsStack.push_back(nextCmd);
                } else if (nextCmd->commandState() == COMMAND_DONE) {
                    // in done queue, can be later undone
                    m_doneCommandsStack.push_back(nextCmd);
                } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                    // in undone queue, can be later redone
                    m_undoneCommandsStack.push_back(nextCmd);
                }

                m_waitingCommandsStack.pop_back();
            }

            m_rwLock.unlockWrite();

            // commands lists updated
            commandUpdate();
        } else {
            System::waitMs(2);
        }

        // yeld
        System::waitMs(0);

        if (!run) {
            break;
        }
    }

    return 0;
}

void CommandManager::begin()
{
    Bool run = False;

    m_rwLock.lockRead();
    run = m_running;
    m_rwLock.unlockRead();

    if (run) {
        return;
    }

    m_running = True;
    m_thread.start();
}

void CommandManager::finish()
{
    m_rwLock.lockWrite();
    m_running = False;
    m_rwLock.unlockWrite();
}

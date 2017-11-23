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

    O3D_ASSERT(m_todoCommandsQueue.empty());
    O3D_ASSERT(m_waitingCommandsQueue.empty());

    Command *cmd = nullptr;
    while (!m_doneCommandsQueue.empty()) {
        cmd = m_doneCommandsQueue.top();
        m_doneCommandsQueue.pop();

        delete cmd;
    }

    while (!m_undoneCommandsQueue.empty()) {
        cmd = m_undoneCommandsQueue.top();
        m_undoneCommandsQueue.pop();

        delete cmd;
    }
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

void CommandManager::onProjectRemoved(const LightRef &ref)
{
    m_rwLock.lockWrite();

    // remove any command related to the project @todo
    std::list<std::stack<Command*>::iterator> eraseList;
    for (auto it = m_todoCommandsQueue.begin(); it != m_todoCommandsQueue.end(); ++it) {
        if ((*it)->targetRef() == ref) {
            eraseList.append(it);
        }
    }

    std::stack<Command*>::iterator it;
    foreach (it, eraseList) {
        delete *it;
        m_todoCommandsQueue.erase(it);
    }

    eraseList.clear();
    for (auto it = m_doneCommandsQueue.begin(); it != m_doneCommandsQueue.end(); ++it) {
        if ((*it)->targetRef() == ref) {
            eraseList.append(it);
        }
    }

    foreach (it, eraseList) {
        delete *it;
        m_doneCommandsQueue.erase(it);
    }

    eraseList.clear();
    for (auto it = m_undoneCommandsQueue.begin(); it != m_undoneCommandsQueue.end(); ++it) {
        if ((*it)->targetRef() == ref) {
            eraseList.append(it);
        }
    }

    foreach (it, eraseList) {
        delete *it;
        m_undoneCommandsQueue.erase(it);
    }

    m_rwLock.unlockWrite();
}

void CommandManager::addCommand(Command *cmd)
{
    Bool update = False;

    m_rwLock.lockWrite();
    m_todoCommandsQueue.push(cmd);

    if (m_undoneCommandsQueue.size()) {
        // clear redo history
        for (auto it = m_undoneCommandsQueue.begin(); it != m_undoneCommandsQueue.end(); ++it) {
            delete *it;
        }

        m_undoneCommandsQueue.clear();
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
    size = m_doneCommandsQueue.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastCmd = m_doneCommandsQueue.pop();
    m_waitingCommandsQueue.push(lastCmd);

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::undoNCommands(Int32 num)
{
    Int32 size = 0;

    m_rwLock.lockRead();
    size = m_doneCommandsQueue.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastCmd = nullptr;
    for (int i = 0; i < std::min(size, num); ++i) {
        lastCmd = m_doneCommandsQueue.pop();
        m_waitingCommandsQueue.push(lastCmd);
    }

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::redoLastCommand()
{
    Int32 size = 0;

    m_rwLock.lockRead();
    size = m_undoneCommandsQueue.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastUndone = m_undoneCommandsQueue.pop();
    m_waitingCommandsQueue.push(lastUndone);

    m_rwLock.unlockWrite();

    commandUpdate();
}

void CommandManager::redoNCommands(Int32 num)
{
    int size = 0;

    m_rwLock.lockRead();
    size = m_undoneCommandsQueue.size();
    m_rwLock.unlockRead();

    if (size == 0) {
        return;
    }

    m_rwLock.lockWrite();

    Command *lastUndone = nullptr;
    for (int i = 0; i < std::min(size, num); ++i) {
        lastUndone = m_undoneCommandsQueue.pop();
        m_waitingCommandsQueue.push(lastUndone);
    }

    m_rwLock.unlockWrite();

    emit commandUpdate();
}

o3d::Bool CommandManager::hasPendingCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_todoCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

o3d::Bool CommandManager::hasRunningCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_waitingCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

o3d::Bool CommandManager::hasDoneCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_doneCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

o3d::Bool CommandManager::hasUndoneCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_undoneCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

o3d::T_StringList CommandManager::undoableCommandList() const
{
    T_StringList cmds;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();

    for (auto cit = m_doneCommandsQueue.cbegin(); cit != m_doneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return cmds;
}

o3d::T_StringList CommandManager::redoableCommandList() const
{
    T_StringList cmds;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();

    for (auto cit = m_undoneCommandsQueue.cbegin(); cit != m_undoneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return cmds;
}

o3d::String CommandManager::nextToUndo() const
{
    String label;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    if (m_doneCommandsQueue.size()) {
        label = m_doneCommandsQueue.top()->commandLabel();
    }
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return label;
}

o3d::String CommandManager::nextToRedo() const
{
    String label;

    m_rwLock->lockRead();
    if (m_undoneCommandsQueue.size()) {
        label = m_undoneCommandsQueue.top()->commandLabel();
    }
    m_rwLock->unlockRead();

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

        run = m_running | !m_waitingCommandsQueue.empty() | !m_todoCommandsQueue.empty();

        // first look in wait list, if empty look in todo list
        if (!m_waitingCommandsQueue.empty()) {
            nextCmd = m_waitingCommandsQueue.pop();
        } else if (!m_todoCommandsQueue.empty()) {
            nextCmd = m_todoCommandsQueue.pop();
        }

        if (nextCmd != nullptr) {
            m_waitingCommandsQueue.push(nextCmd);
        }

        m_rwLock.unlockWrite();

        if (nextCmd != nullptr) {
            error = false;

            try {
                if (nextCmd->commandState() == COMMAND_READY) {
                    // never executed, then execute
                    if (nextCmd->doCommand()) {
                        nextCmd->setDone();

                        emit commandDone(nextCmd->commandName(), nextCmd->commandLabel(), true);
                    }
                } else if (nextCmd->commandState() == COMMAND_REDONE) {
                    // previously redone, then undo
                    if (nextCmd->undoCommand()) {
                        nextCmd->setUndone();

                        emit commandDone(nextCmd->commandName(), nextCmd->commandLabel(), false);
                    }
                } else if (nextCmd->commandState() == COMMAND_DONE) {
                    // previously executed, then undo
                    if (nextCmd->undoCommand()) {
                        nextCmd->setUndone();

                        emit commandDone(nextCmd->commandName(), nextCmd->commandLabel(), false);
                    }
                } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                    // previously undone, then redo
                    if (nextCmd->redoCommand()) {
                        nextCmd->setReDone();

                        emit commandDone(nextCmd->commandName(), nextCmd->commandLabel(), true);
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
                for (auto it = m_waitingCommandsQueue.begin(); it != m_waitingCommandsQueue.end(); ++it) {
                    delete *it;
                }

                m_waitingCommandsQueue.clear();

                // and cannot performs next pending commands because of the possible coherency, so delete them to
                for (auto it = m_todoCommandsQueue.begin(); it != m_todoCommandsQueue.end(); ++it) {
                    delete *it;
                }

                m_todoCommandsQueue.clear();
            } else {
                if (nextCmd->commandState() == COMMAND_READY) {
                    // error, may not arrives
                } else if (nextCmd->commandState() == COMMAND_REDONE) {
                    // in done queue, can be later undone
                    m_doneCommandsQueue.push(nextCmd);
                } else if (nextCmd->commandState() == COMMAND_DONE) {
                    // in done queue, can be later undone
                    m_doneCommandsQueue.push(nextCmd);
                } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                    // in undone queue, can be later redone
                    m_undoneCommandsQueue.push(nextCmd);
                }

                m_waitingCommandsQueue.pop();
            }

            m_rwLock.unlockWrite();

            // commands lists updated
            commandUpdate();
        } else {
            msleep(2);
        }

        yieldCurrentThread();

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
    start();
}

void CommandManager::finish()
{
    m_rwLock.lockWrite();
    m_running = False;
    m_rwLock.unlockWrite();
}

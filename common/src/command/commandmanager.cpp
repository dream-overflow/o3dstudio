/**
 * @brief Common command manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include "common/command/commandmanager.h"

using namespace o3d::studio::common;


CommandManager::CommandManager() :
    m_running{false}
{
}

CommandManager::~CommandManager()
{
    Q_ASSERT(m_todoCommandsQueue.isEmpty());
    Q_ASSERT(m_waitingCommandsQueue.isEmpty());

    Command *cmd = nullptr;
    foreach (cmd, m_doneCommandsQueue) {
        delete cmd;
    }

    cmd = nullptr;
    foreach (cmd, m_undoneCommandsQueue) {
        delete cmd;
    }
}

void CommandManager::addCommand(Command *cmd)
{
    m_todoCommandsQueue.enqueue(cmd);
}

void CommandManager::undoLastCommand()
{
    if (m_doneCommandsQueue.size() == 0) {
        return;
    }

    Command *lastCmd = m_doneCommandsQueue.dequeue();
    m_waitingCommandsQueue.enqueue(lastCmd);
}

void CommandManager::undoNCommands(int num)
{

}

void CommandManager::redoLastCommand()
{
    if (m_undoneCommandsQueue.size() == 0) {
        return;
    }

    Command *lastUndone = m_undoneCommandsQueue.dequeue();
    m_waitingCommandsQueue.enqueue(lastUndone);
}

void CommandManager::redoNCommands(int num)
{

}

void CommandManager::setCommandDone(Command *cmd)
{
    // @todo
}

bool CommandManager::hasPendingCommands() const
{
    return !m_todoCommandsQueue.isEmpty();
}

bool CommandManager::hasRunningCommands() const
{
    return !m_waitingCommandsQueue.isEmpty();
}

bool CommandManager::hasDoneCommands() const
{
    return !m_doneCommandsQueue.isEmpty();
}

bool CommandManager::hasUndoneCommands() const
{
    return !m_undoneCommandsQueue.isEmpty();
}

QStringList CommandManager::undoableCommandList() const
{
    QStringList cmds;

    for (auto cit = m_doneCommandsQueue.cbegin(); cit != m_doneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    return cmds;
}

QStringList CommandManager::redoableCommandList() const
{
    QStringList cmds;

    for (auto cit = m_undoneCommandsQueue.cbegin(); cit != m_undoneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    return cmds;
}

void CommandManager::run()
{
    bool run = true;

    while (1) {
        Command *nextCmd = nullptr;

        m_rwLock.lockForWrite();

        run = m_running | !m_waitingCommandsQueue.isEmpty() | !m_todoCommandsQueue.isEmpty();

        // first look in wait list, if empty look in todo list
        if (!m_waitingCommandsQueue.isEmpty()) {
            nextCmd = m_waitingCommandsQueue.dequeue();
        } else if (!m_todoCommandsQueue.isEmpty()) {
            nextCmd = m_todoCommandsQueue.dequeue();
        }

        if (nextCmd != nullptr) {
            m_waitingCommandsQueue.enqueue(nextCmd);
        }

        m_rwLock.unlock();

        if (nextCmd != nullptr) {
            if (nextCmd->commandState() == COMMAND_READY) {
                // never executed, then execute
                nextCmd->executeCommand();
            } else if (nextCmd->commandState() == COMMAND_DONE) {
                // previously redone, then undo
                nextCmd->undoCommand();
            } else if (nextCmd->commandState() == COMMAND_EXECUTED) {
                // previously executed, then undo
                nextCmd->undoCommand();
            } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                // previously undone, then redo
                nextCmd->redoCommand();
            }

            m_rwLock.lockForWrite();

            if (nextCmd->commandState() == COMMAND_READY) {
                // error, may not arrives
            } else if (nextCmd->commandState() == COMMAND_DONE) {
                // in done queue, can be later undone
                m_doneCommandsQueue.enqueue(nextCmd);
            } else if (nextCmd->commandState() == COMMAND_EXECUTED) {
                // in done queue, can be later undone
                m_doneCommandsQueue.enqueue(nextCmd);
            } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                // in undone queue, can be later redone
                m_undoneCommandsQueue.enqueue(nextCmd);
            }

            m_rwLock.unlock();
        }

        yieldCurrentThread();

        if (!run) {
            break;
        }
    }
}

void CommandManager::begin()
{
    bool run = false;

    m_rwLock.lockForRead();
    run = m_running;
    m_rwLock.unlock();

    if (run) {
        return;
    }

    m_running = true;
    start();
}

void CommandManager::finish()
{
    m_rwLock.lockForWrite();
    m_running = false;
    m_rwLock.unlock();
}

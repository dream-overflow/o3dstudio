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
    m_rwLock.lockForWrite();
    m_todoCommandsQueue.enqueue(cmd);
    m_rwLock.unlock();
}

void CommandManager::undoLastCommand()
{
    int size = 0;

    m_rwLock.lockForRead();
    size = m_doneCommandsQueue.size();
    m_rwLock.unlock();

    if (size == 0) {
        return;
    }

    m_rwLock.lockForWrite();

    Command *lastCmd = m_doneCommandsQueue.dequeue();
    m_waitingCommandsQueue.enqueue(lastCmd);

    m_rwLock.unlock();
}

void CommandManager::undoNCommands(int num)
{

}

void CommandManager::redoLastCommand()
{
    int size = 0;

    m_rwLock.lockForRead();
    size = m_undoneCommandsQueue.size();
    m_rwLock.unlock();

    if (size == 0) {
        return;
    }

    m_rwLock.lockForWrite();

    Command *lastUndone = m_undoneCommandsQueue.dequeue();
    m_waitingCommandsQueue.enqueue(lastUndone);

    m_rwLock.unlock();
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
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_todoCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

bool CommandManager::hasRunningCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_waitingCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

bool CommandManager::hasDoneCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_doneCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

bool CommandManager::hasUndoneCommands() const
{
    int size = 0;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    size = m_undoneCommandsQueue.size();
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return size > 0;
}

QStringList CommandManager::undoableCommandList() const
{
    QStringList cmds;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();

    for (auto cit = m_doneCommandsQueue.cbegin(); cit != m_doneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return cmds;
}

QStringList CommandManager::redoableCommandList() const
{
    QStringList cmds;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();

    for (auto cit = m_undoneCommandsQueue.cbegin(); cit != m_undoneCommandsQueue.cend(); ++cit) {
        cmds.append((*cit)->commandLabel());
    }

    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

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

            m_rwLock.lockForWrite();

            if (nextCmd->commandState() == COMMAND_READY) {
                // error, may not arrives
            } else if (nextCmd->commandState() == COMMAND_REDONE) {
                // in done queue, can be later undone
                m_doneCommandsQueue.enqueue(nextCmd);
            } else if (nextCmd->commandState() == COMMAND_DONE) {
                // in done queue, can be later undone
                m_doneCommandsQueue.enqueue(nextCmd);
            } else if (nextCmd->commandState() == COMMAND_UNDONE) {
                // in undone queue, can be later redone
                m_undoneCommandsQueue.enqueue(nextCmd);
            }

            m_waitingCommandsQueue.dequeue();
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

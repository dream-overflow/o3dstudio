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


CommandManager::CommandManager() :
    m_running(false)
{
}

CommandManager::~CommandManager()
{
    wait();  // join the thread

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

void CommandManager::initialize()
{
    // initial setup of current workspace
    common::WorkspaceManager *workspaceManager = &common::Application::instance()->workspaces();
    connect(workspaceManager, SIGNAL(onWorkspaceActivated(QString)), SLOT(onChangeCurrentWorkspace(QString)));

    onChangeCurrentWorkspace(workspaceManager->current()->name());

}

void CommandManager::onChangeCurrentWorkspace(const QString &name)
{
    Q_UNUSED(name)

    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        connect(workspace, SIGNAL(onProjectRemoved(const LightRef &)), SLOT(onProjectRemoved(const LightRef &)));
    }
}

void CommandManager::onProjectRemoved(const LightRef &ref)
{
    m_rwLock.lockForWrite();

    // remove any command related to the project @todo
    QList<QStack<Command*>::iterator> eraseList;
    for (auto it = m_todoCommandsQueue.begin(); it != m_todoCommandsQueue.end(); ++it) {
        if ((*it)->targetRef() == ref) {
            eraseList.append(it);
        }
    }

    QStack<Command*>::iterator it;
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

    m_rwLock.unlock();
}

void CommandManager::addCommand(Command *cmd)
{
    bool update = false;

    m_rwLock.lockForWrite();
    m_todoCommandsQueue.push(cmd);

    if (m_undoneCommandsQueue.size()) {
        // clear redo history
        for (auto it = m_undoneCommandsQueue.begin(); it != m_undoneCommandsQueue.end(); ++it) {
            delete *it;
        }

        m_undoneCommandsQueue.clear();
        update = true;
    }

    m_rwLock.unlock();

    if (update) {
        emit commandUpdate();
    }
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

    Command *lastCmd = m_doneCommandsQueue.pop();
    m_waitingCommandsQueue.push(lastCmd);

    m_rwLock.unlock();

    emit commandUpdate();
}

void CommandManager::undoNCommands(int num)
{
    int size = 0;

    m_rwLock.lockForRead();
    size = m_doneCommandsQueue.size();
    m_rwLock.unlock();

    if (size == 0) {
        return;
    }

    m_rwLock.lockForWrite();

    Command *lastCmd = nullptr;
    for (int i = 0; i < std::min(size, num); ++i) {
        lastCmd = m_doneCommandsQueue.pop();
        m_waitingCommandsQueue.push(lastCmd);
    }

    m_rwLock.unlock();

    emit commandUpdate();
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

    Command *lastUndone = m_undoneCommandsQueue.pop();
    m_waitingCommandsQueue.push(lastUndone);

    m_rwLock.unlock();

    emit commandUpdate();
}

void CommandManager::redoNCommands(int num)
{
    int size = 0;

    m_rwLock.lockForRead();
    size = m_undoneCommandsQueue.size();
    m_rwLock.unlock();

    if (size == 0) {
        return;
    }

    m_rwLock.lockForWrite();

    Command *lastUndone = nullptr;
    for (int i = 0; i < std::min(size, num); ++i) {
        lastUndone = m_undoneCommandsQueue.pop();
        m_waitingCommandsQueue.push(lastUndone);
    }

    m_rwLock.unlock();

    emit commandUpdate();
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

QString CommandManager::nextToUndo() const
{
    QString label;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    if (m_doneCommandsQueue.size()) {
        label = m_doneCommandsQueue.top()->commandLabel();
    }
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return label;
}

QString CommandManager::nextToRedo() const
{
    QString label;

    const_cast<QReadWriteLock*>(&m_rwLock)->lockForRead();
    if (m_undoneCommandsQueue.size()) {
        label = m_undoneCommandsQueue.top()->commandLabel();
    }
    const_cast<QReadWriteLock*>(&m_rwLock)->unlock();

    return label;
}

void CommandManager::run()
{
    bool run = true;
    bool error = false;
    Messenger& messenger = Application::instance()->messenger();

    while (1) {
        Command *nextCmd = nullptr;

        m_rwLock.lockForWrite();

        run = m_running | !m_waitingCommandsQueue.isEmpty() | !m_todoCommandsQueue.isEmpty();

        // first look in wait list, if empty look in todo list
        if (!m_waitingCommandsQueue.isEmpty()) {
            nextCmd = m_waitingCommandsQueue.pop();
        } else if (!m_todoCommandsQueue.isEmpty()) {
            nextCmd = m_todoCommandsQueue.pop();
        }

        if (nextCmd != nullptr) {
            m_waitingCommandsQueue.push(nextCmd);
        }

        m_rwLock.unlock();

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
            } catch (BaseException &e) {
                messenger.critical(e.message());

                error = true;
            }

            m_rwLock.lockForWrite();

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

            m_rwLock.unlock();

            // commands lists updated
            emit commandUpdate();
        } else {
            msleep(2);
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

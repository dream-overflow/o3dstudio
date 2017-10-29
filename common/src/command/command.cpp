/**
 * @brief Common command base
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include "common/command/command.h"

using namespace o3d::studio::common;


Command::Command(const QString& name, const QMap<QString, QVariant> &options) :
    m_commandName(name),
    m_commandState(COMMAND_READY),
    m_commandOptions(options)
{
}

Command::~Command()
{

}

QString Command::commandName() const
{
    return m_commandName;
}

CommandState Command::commandState() const
{
    return m_commandState;
}

const QMap<QString, QVariant> &Command::commandOptions() const
{
    return m_commandOptions;
}

QString Command::commandLabel() const
{
    return m_commandName;
}

void Command::setExecuted()
{
    m_commandState = COMMAND_EXECUTED;
}

void Command::setDone()
{
    m_commandState = COMMAND_DONE;
}

void Command::setUndone()
{
    m_commandState = COMMAND_UNDONE;
}

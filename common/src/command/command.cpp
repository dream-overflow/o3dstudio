/**
 * @brief Common command base
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include "o3d/studio/common/command/command.h"

using namespace o3d::studio::common;


Command::Command(const String& name, const LightRef &targetRef) :
    m_targetRef(targetRef),
    m_commandName(name),
    m_commandState(COMMAND_READY)
{
}

Command::~Command()
{

}

o3d::String Command::commandName() const
{
    return m_commandName;
}

CommandState Command::commandState() const
{
    return m_commandState;
}

o3d::String Command::commandLabel() const
{
    return m_commandName;
}

const LightRef &Command::targetRef() const
{
    return m_targetRef;
}

void Command::setDone()
{
    m_commandState = COMMAND_DONE;
}

void Command::setReDone()
{
    m_commandState = COMMAND_REDONE;
}

void Command::setUndone()
{
    m_commandState = COMMAND_UNDONE;
}

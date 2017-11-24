/**
 * @brief A simple dummy command, does nothing.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include <QtWidgets/QApplication>

#include "o3d/studio/common/command/dummycommand.h"

using namespace o3d::studio::common;


DummyCommand::DummyCommand() :
    Command("o3s::common::dummy", LightRef())
{
}

DummyCommand::~DummyCommand()
{
}

o3d::String DummyCommand::commandLabel() const
{
    return fromQString(tr("Dummy operation"));
}

o3d::Bool DummyCommand::doCommand()
{
    return True;
}

o3d::Bool DummyCommand::undoCommand()
{
    return True;
}

o3d::Bool DummyCommand::redoCommand()
{
    return True;
}

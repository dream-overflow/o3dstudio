/**
 * @brief A simple dummy command, does nothing.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include <QtWidgets/QApplication>

#include "common/command/dummycommand.h"

using namespace o3d::studio::common;


DummyCommand::DummyCommand(const QMap<QString, QVariant> &options) :
    Command("o3s::common::dummy", options)
{
}

DummyCommand::~DummyCommand()
{
}

QString DummyCommand::commandLabel() const
{
    return QApplication::translate("o3d::studio::common", "Dummy operation");
}

bool DummyCommand::executeCommand()
{
    qDebug("DummyCommand executed");
    return true;
}

bool DummyCommand::undoCommand()
{
    qDebug("DummyCommand undone");
    return true;
}

bool DummyCommand::redoCommand()
{
    qDebug("DummyCommand redone");
    return true;
}

/**
 * @brief A simple dummy command, does nothing.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_DUMMYCOMMAND_H
#define _O3DS_COMMON_DUMMYCOMMAND_H

#include <QtCore/QCoreApplication>

#include "command.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API DummyCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(DummyCommand)

public:

    DummyCommand();
    virtual ~DummyCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DUMMYCOMMAND_H

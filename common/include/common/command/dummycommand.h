/**
 * @brief A simple dummy command, does nothing.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_DUMMYCOMMAND_H
#define _O3DS_COMMON_DUMMYCOMMAND_H

#include "command.h"

namespace o3d {
namespace studio {
namespace common {

class DummyCommand : public Command
{
public:

    DummyCommand();
    virtual ~DummyCommand();

    virtual QString commandLabel() const override;
    virtual bool doCommand() override;
    virtual bool undoCommand() override;
    virtual bool redoCommand() override;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DUMMYCOMMAND_H

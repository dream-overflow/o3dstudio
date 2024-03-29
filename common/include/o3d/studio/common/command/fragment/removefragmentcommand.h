/**
 * @brief Remove fragment command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-15
 * @details
 */

#ifndef _O3DS_COMMON_REMOVEFRAGMENTCOMMAND_H
#define _O3DS_COMMON_REMOVEFRAGMENTCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <o3d/core/templatearray.h>

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class O3S_API RemoveFragmentCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(RemoveFragmentCommand)

public:

    RemoveFragmentCommand(const LightRef &fragmentRef, const LightRef &parentRef);
    virtual ~RemoveFragmentCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    LightRef m_parent;
    LightRef m_fragment;

    ArrayUInt8 m_data;
    ObjectRef m_storedFragmentRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_REMOVEFRAGMENTCOMMAND_H

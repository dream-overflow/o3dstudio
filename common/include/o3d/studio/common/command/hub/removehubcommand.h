/**
 * @brief Remove hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-14
 * @details
 */

#ifndef _O3DS_COMMON_REMOVEHUBCOMMAND_H
#define _O3DS_COMMON_REMOVEHUBCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <o3d/core/templatearray.h>

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class O3S_API RemoveHubCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(RemoveHubCommand)

public:

    RemoveHubCommand(const LightRef &hubRef, const LightRef &parentRef);
    virtual ~RemoveHubCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    LightRef m_parent;
    LightRef m_hub;

    ArrayUInt8 m_data;
    ObjectRef m_storedHubRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_REMOVEHUBCOMMAND_H

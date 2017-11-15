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

    virtual QString commandLabel() const override;
    virtual bool doCommand() override;
    virtual bool undoCommand() override;
    virtual bool redoCommand() override;

private:

    LightRef m_parent;
    LightRef m_hub;

    QByteArray m_data;
    ObjectRef m_storedHubRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_REMOVEHUBCOMMAND_H

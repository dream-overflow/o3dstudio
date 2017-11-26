/**
 * @brief Remove asset command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#ifndef _O3DS_COMMON_REMOVEASSETCOMMAND_H
#define _O3DS_COMMON_REMOVEASSETCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <o3d/core/templatearray.h>

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class O3S_API RemoveAssetCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(RemoveAssetCommand)

public:

    RemoveAssetCommand(const LightRef &assetRef, const LightRef &parentRef);
    virtual ~RemoveAssetCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    LightRef m_parent;
    LightRef m_asset;

    ArrayUInt8 m_data;
    ObjectRef m_storedAssetRef;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_REMOVEASSETCOMMAND_H

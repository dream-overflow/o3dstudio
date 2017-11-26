/**
 * @brief Add asset command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#ifndef _O3DS_COMMON_ADDASSETCOMMAND_H
#define _O3DS_COMMON_ADDASSETCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class Project;

class O3S_API AddAssetCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(AddAssetCommand)

public:

    AddAssetCommand(const LightRef &project, const String &name);
    virtual ~AddAssetCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    LightRef m_project;
    ObjectRef m_asset;

    String m_assetName;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ADDASSETCOMMAND_H

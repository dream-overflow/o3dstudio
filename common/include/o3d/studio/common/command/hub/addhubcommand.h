/**
 * @brief Add hub command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#ifndef _O3DS_COMMON_ADDHUBCOMMAND_H
#define _O3DS_COMMON_ADDHUBCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class Project;

class O3S_API AddHubCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(AddHubCommand)

public:

    AddHubCommand(const LightRef &parent, const TypeRef& componentType, const String &name);
    virtual ~AddHubCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    TypeRef m_componentType;

    LightRef m_parent;
    ObjectRef m_storedHubRef;

    String m_hubName;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ADDHUBCOMMAND_H

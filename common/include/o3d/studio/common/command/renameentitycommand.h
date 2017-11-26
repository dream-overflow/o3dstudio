/**
 * @brief Rename an entity command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-26
 * @details
 */

#ifndef _O3DS_COMMON_RENAMEENTITYCOMMAND_H
#define _O3DS_COMMON_RENAMEENTITYCOMMAND_H

#include "command.h"
#include "../objectref.h"

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class Project;

class O3S_API RenameEntityCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(RenameEntityCommand)

public:

    RenameEntityCommand(const LightRef &entityRef, const LightRef &parent, const String &name);
    virtual ~RenameEntityCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    TypeRef m_entityType;

    LightRef m_parent;

    String m_entityName;
    String m_storedName;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_RENAMEENTITYCOMMAND_H

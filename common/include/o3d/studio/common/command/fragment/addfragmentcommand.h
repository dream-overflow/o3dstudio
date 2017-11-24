/**
 * @brief Add fragment command
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#ifndef _O3DS_COMMON_ADDFRAGMENTCOMMAND_H
#define _O3DS_COMMON_ADDFRAGMENTCOMMAND_H

#include "../command.h"
#include "../../objectref.h"

#include <QtCore/QCoreApplication>

namespace o3d {
namespace studio {
namespace common {

class Project;

class O3S_API AddFragmentCommand : public Command
{
    Q_DECLARE_TR_FUNCTIONS(AddFragmentCommand)

public:

    AddFragmentCommand(const LightRef &project, const String &name);
    virtual ~AddFragmentCommand();

    virtual String commandLabel() const override;
    virtual Bool doCommand() override;
    virtual Bool undoCommand() override;
    virtual Bool redoCommand() override;

private:

    LightRef m_project;
    ObjectRef m_fragment;

    String m_fragmentName;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_ADDFRAGMENTCOMMAND_H

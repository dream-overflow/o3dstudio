/**
 * @brief Common scene command class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-16
 * @details
 */

#ifndef _O3DS_COMMON_SCENECOMMAND_H
#define _O3DS_COMMON_SCENECOMMAND_H

#include "../exception.h"
#include "../objectref.h"

namespace o3d {
namespace studio {
namespace common {

class MasterScene;
class Entity;
class Hub;

/**
 * @brief The SceneCommand base class
 */
class O3S_API SceneCommand
{
public:

    virtual ~SceneCommand() = 0;
    virtual void process(MasterScene *masterScene) = 0;
};

/**
 * @brief The SceneHubCommand class
 */
class O3S_API SceneHubCommand : public SceneCommand
{
public:

    enum CommandType
    {
        CREATE = 0,
        DELETE = 1,
        SYNC = 2
    };

    SceneHubCommand(Hub *hub, CommandType cmdType);
    virtual ~SceneHubCommand();

    virtual void process(MasterScene *masterScene) override;

protected:

    CommandType m_cmdType;

    Hub *m_hub;
    LightRef m_ref;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SCENECOMMAND_H

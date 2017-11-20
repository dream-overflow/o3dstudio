/**
 * @brief Common messenger manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_MESSENGER_H
#define _O3DS_COMMON_MESSENGER_H

#include <map>

#include <o3d/core/baseobject.h>
#include <o3d/core/mutex.h>
#include <o3d/core/datetime.h>
#include <o3d/core/evt.h>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Thread protected messenger reveicer and dispatcher.
 */
class O3S_API Messenger : public BaseObject
{
public:

    enum MessageType
    {
        DEBUG_MSG = 0,
        WARNING_MSG,
        CRITICAL_MSG,
        FATAL_MSG,
        INFO_MSG
    };

    Messenger(BaseObject *parent = nullptr);
    virtual ~Messenger();

    void message(UInt32 msgType, const String &message);

    void debug(const String &message);
    void info(const String &message);
    void warning(const String &message);
    void fatal(const String &message);
    void critical(const String &message);

public:

    Signal<UInt32, const String&> onNewMessage{this};

protected:

    class Message
    {
    public:

        DateTime dateTime;
        UInt32 msgType;
        String message;
    };

    FastMutex m_mutex;
    std::map<DateTime, Message> m_messages;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MESSENGER_H

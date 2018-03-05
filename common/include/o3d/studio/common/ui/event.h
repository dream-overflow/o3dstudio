/**
 * @brief Common base ui event
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-25
 * @details
 */

#ifndef _O3DS_COMMON_EVENT_H
#define _O3DS_COMMON_EVENT_H

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The Event base class with a type of event.
 */
class O3S_API Event
{
public:

    enum Type
    {
        KEY_PRESS_EVENT = 1,
        KEY_RELEASE_EVENT = 2,
        MOUSE_BUTTON_PRESS = 10,
        MOUSE_BUTTON_RELEASE = 11,
        MOUSE_MOVE = 12,
        MOUSE_BUTTON_DBL_CLICK = 13,
        WHEEL_EVENT = 20,
        HOVER_ENTER = 30,
        HOVER_LEAVE = 31,
        HOVER_MOVE = 32,
        FOCUS_IN = 40,
        FOCUS_OUT = 41,
        ENTER = 50,
        LEAVE = 51,
        DRAG_ENTER = 60,
        DRAG_LEAVE = 61,
        DRAG_MOVE = 62,
        DROP = 63,
        TABLET_MOVE = 70,
        TABLET_PRESS = 71,
        TABLET_RELEASE = 72,
        TABLET_ENTER_PROXIMITY = 73,
        TABLET_LEAVE_PROXIMITY = 74,
        TABLET_TRACKING_CHANGE = 75,
        TOUCH_BEGIN = 80,
        TOUCH_CANCEL = 81,
        TOUCH_END = 82,
        TOUCH_UPDATE = 83
    };

    Event(Type type) :
        m_type(type)
    {
    }

protected:

    Type m_type;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_EVENT_H

/**
 * @brief Common base ui input event
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-25
 * @details
 */

#ifndef _O3DS_COMMON_INPUTEVENT_H
#define _O3DS_COMMON_INPUTEVENT_H

#include "event.h"
#include <o3d/core/types.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The InputEvent base class defining a list of modifiers keys, and a timestamp.
 */
class O3S_API InputEvent : public Event
{
public:

    enum KeyboardModifier
    {
        NO_MODIFIER     = 0,            //!< No modifier key is pressed.
        SHIFT_MODIFIER  = 0x02000000,   //!< A Shift key on the keyboard is pressed.
        CTRL_MODIFIERD  = 0x04000000,   //!< A Ctrl key on the keyboard is pressed.
        ALT_MODIFIER    = 0x08000000,   //!< An Alt key on the keyboard is pressed.
        META_MODIFIER   = 0x10000000,   //!< A Meta key on the keyboard is pressed.
        KEYPAD_MODIFIER = 0x20000000    //!< A keypad button is pressed.
    };

    InputEvent(Type type, KeyboardModifier modifiers, UInt64 timestamp) :
        Event(type),
        m_modifiers(modifiers),
        m_timestamp(timestamp)
    {

    }

    inline KeyboardModifier modifiers() const { return m_modifiers; }
    inline UInt64 timestamp() const { return m_timestamp; }

protected:

    KeyboardModifier m_modifiers;
    UInt64 m_timestamp;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_INPUTEVENT_H

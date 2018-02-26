/**
 * @brief Common key ui input event
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-25
 * @details
 */

#ifndef _O3DS_COMMON_KEYEVENT_H
#define _O3DS_COMMON_KEYEVENT_H

#include "inputevent.h"

#include <o3d/core/string.h>
#include <o3d/core/keyboard.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The KeyEvent class.
 */
class O3S_API KeyEvent : public InputEvent
{
public:

    KeyEvent(Type type,
             UInt64 timestamp,
             Int32 key,
             KeyboardModifier modifiers,
             // UInt32 nativeScanCode,
             UInt32 nativeVirtualKey,
             // UInt32 nativeModifiers,
             const String &text = String(),
             Bool autorep = False,
             UInt16 count = 1) :
        InputEvent(type, modifiers, timestamp),
        m_key(key),
        m_text(text),
        m_autorep(autorep),
        m_count(count),
        m_vkey(nativeVirtualKey)
    {
    }

    inline Int32 count() const { return m_count; }
    inline Bool isAutoRepeat() const { return m_autorep; }
    inline Int32 key() const { return m_key; }
    // inline Bool matches(VKey key) const
    // inline UInt32 nativeModifiers() const
    // inline UInt32 scanCode() const { return m_scancode; }
    inline VKey vKey() const { return (VKey)m_vkey; }
    inline String text() const { return m_text; }

protected:

    Int32 m_key;
    String m_text;
    Bool m_autorep;
    UInt16 m_count;

    UInt32/*VKey*/ m_vkey;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_KEYEVENT_H

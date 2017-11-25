/**
 * @brief Common mouse wheel ui input event
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-25
 * @details
 */

#ifndef _O3DS_COMMON_MOUSEWHEEL_H
#define _O3DS_COMMON_MOUSEWHEEL_H

#include "inputevent.h"

#include <o3d/core/string.h>
#include <o3d/core/mouse.h>
#include <o3d/core/vector2.h>

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The MouseEvent class.
 */
class O3S_API WheelEvent : public InputEvent
{
public:

    WheelEvent(Type type,
               UInt64 timestamp,
               KeyboardModifier modifiers,
               UInt32 buttons,
               const Point2i &angleDelta,
               const Point2f &globalPos,
               const Point2f &pos) :
        InputEvent(type, modifiers, timestamp),
        m_buttons(buttons),
        m_angleDelta(angleDelta),
        m_globalPos(globalPos),
        m_pos(pos)
    {
    }

    inline const Point2i& angleDelta() const { return m_angleDelta; }

    inline UInt32 buttons() const { return m_buttons; }

    inline Point2i pos() const
    {
        return Point2i((Int32)std::round(m_pos.x()), (Int32)std::round(m_pos.y()));
    }
    inline Point2i globalPos() const
    {
        return Point2i((Int32)std::round(m_globalPos.x()), (Int32)std::round(m_globalPos.y()));
    }

    inline const Point2f& posF() const { return m_pos; }
    inline const Point2f& globalPosF() const { return m_globalPos; }

    inline Bool button(Mouse::Buttons btn) const
    {
        return (m_buttons & (1 << (int)btn)) > 0;
    }

protected:

    UInt32 m_buttons;

    Point2i m_angleDelta;

    Point2f m_globalPos;
    Point2f m_pos;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MOUSEWHEEL_H

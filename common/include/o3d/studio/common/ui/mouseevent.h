/**
 * @brief Common mouse ui input event
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-25
 * @details
 */

#ifndef _O3DS_COMMON_MOUSEEVENT_H
#define _O3DS_COMMON_MOUSEEVENT_H

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
class O3S_API MouseEvent : public InputEvent
{
public:

    MouseEvent(Type type,
               UInt64 timestamp,
               KeyboardModifier modifiers,
               UInt32 button,
               UInt32 buttons,
               const Point2i &globalPos,
               const Point2f &localPos,
               const Point2f &screenPos,
               const Point2f &windowPos) :
        InputEvent(type, modifiers, timestamp),
        m_button(button),
        m_buttons(buttons),
        m_globalPos(globalPos),
        m_localPos(localPos),
        m_screenPos(screenPos),
        m_windowPos(windowPos)
    {
    }

    inline UInt32 buttons() const { return m_buttons; }

    inline const Point2f& localPos() const { return m_localPos; }
    inline const Point2i& globalPos() const { return m_globalPos; }

    inline const Point2f& screenPos() const { return m_screenPos; }
    inline const Point2f& windowPos() const { return m_windowPos; }

    inline Point2i pos() const
    {
        return Point2i((Int32)std::round(m_localPos.x()), (Int32)std::round(m_localPos.y()));
    }

    inline Bool button(Mouse::Buttons btn) const
    {
        return BUTTONS_MAPPING[(unsigned int)btn] == (unsigned int)m_button;
    }

protected:

    const unsigned int BUTTONS_MAPPING[Mouse::NUM_BUTTONS] = {
        Qt::LeftButton,    // Mouse::LEFT,
        Qt::RightButton,   // Mouse::RIGHT,
        Qt::MiddleButton,  // Mouse::MIDDLE,
        Qt::XButton1,      // Mouse::X1,
        Qt::TaskButton,    // Mouse::UNKNOWN,
        Qt::XButton2       // Mouse::X2
    };

    UInt32 m_button;
    UInt32 m_buttons;

    Point2i m_globalPos;
    Point2f m_localPos;

    Point2f m_screenPos;
    Point2f m_windowPos;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MOUSEEVENT_H

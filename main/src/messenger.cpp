/**
 * @brief Common messenger manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/messenger.h"

#include <o3d/core/debug.h>

using namespace o3d::studio::common;


Messenger::Messenger(BaseObject *parent) :
    BaseObject(parent)
{
    // async to avoid threading issues
    o3d::Debug::instance()->throwAll.connect(this, &Messenger::onDebugInfo);
}

Messenger::~Messenger()
{

}

void Messenger::message(UInt32 msgType, const String &message)
{
    Message lmessage;
    lmessage.dateTime = DateTime(True);
    lmessage.msgType = msgType;
    lmessage.message = message;

    m_mutex.lock();
    m_messages.push_back(lmessage);

    if (m_messages.size() > 1000) {
        m_messages.pop_front();
    }

    m_mutex.unlock();

    onNewMessage(lmessage.msgType, lmessage.message);
}

void Messenger::debug(const String &message)
{
    this->message(DEBUG_MSG, message);
}

void Messenger::status(const o3d::String &message)
{
    this->message(STATUS_MSG, message);
}

void Messenger::info(const String &message)
{
    this->message(INFO_MSG, message);
}

void Messenger::warning(const String &message)
{
    this->message(WARNING_MSG, message);
}

void Messenger::fatal(const String &message)
{
    this->message(FATAL_MSG, message);
}

void Messenger::critical(const String &message)
{
    this->message(CRITICAL_MSG, message);
}

void Messenger::onDebugInfo(o3d::DebugInfo _info)
{
    switch (_info.Type) {
        case o3d::DebugInfo::MESSAGE_INFO:
            info(_info.Message);
            break;

        case o3d::DebugInfo::WARNING_INFO:
            warning(_info.Message);
            break;

        case o3d::DebugInfo::CRITICAL_INFO:
            critical(_info.Message);
            break;

        case o3d::DebugInfo::ERROR_INFO:
            fatal(_info.Message);
            break;

        default:
            break;
    }
}

/**
 * @brief Common messenger manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::common;


Messenger::Messenger(BaseObject *parent) :
    BaseObject(parent)
{

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
    m_messages[lmessage.dateTime] = lmessage;
    m_mutex.unlock();

    onNewMessage(lmessage.msgType, lmessage.message);
}

void Messenger::debug(const String &message)
{
    this->message(DEBUG_MSG, message);
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

/**
 * @brief Common messenger manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#include "o3d/studio/common/messenger.h"

using namespace o3d::studio::common;


Messenger::Messenger(QObject *parent) :
    QObject(parent)
{

}

Messenger::~Messenger()
{

}

void Messenger::message(QtMsgType msgType, const QString &message)
{
    // @todo list
    emit onNewMessage(msgType, message);
}

void Messenger::debug(const QString &message)
{
    this->message(QtDebugMsg, message);
}

void Messenger::info(const QString &message)
{
    this->message(QtInfoMsg, message);
}

void Messenger::warning(const QString &message)
{
    this->message(QtWarningMsg, message);
}

void Messenger::fatal(const QString &message)
{
    this->message(QtFatalMsg, message);
}

void Messenger::critical(const QString &message)
{
    this->message(QtCriticalMsg, message);
}

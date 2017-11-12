/**
 * @brief Common messenger manager
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_MESSENGER_H
#define _O3DS_COMMON_MESSENGER_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QMutex>
#include <QtCore/QMap>
#include <QtCore/QStringList>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief Thread protected messenger reveicer and dispatcher.
 */
class O3S_API Messenger : public QObject
{
    Q_OBJECT

public:

    Messenger(QObject *parent = nullptr);
    virtual ~Messenger();

    void message(QtMsgType msgType, const QString &message);

    void debug(const QString &message);
    void info(const QString &message);
    void warning(const QString &message);
    void fatal(const QString &message);
    void critical(const QString &message);

signals:

    void onNewMessage(QtMsgType msgType, const QString &message);

protected:

    class Message
    {
    public:

        QDateTime dateTime;
        QtMsgType msgType;
        QString message;
    };

    QMutex m_mutex;
    QMap<QDateTime, Message> m_messages;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MESSENGER_H

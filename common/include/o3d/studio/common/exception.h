/**
 * @brief Common base exceptions
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-04
 * @details
 */

#ifndef _O3DS_COMMON_EXCEPTION_H
#define _O3DS_COMMON_EXCEPTION_H

#include <QtCore/QException>
#include <QtCore/QCoreApplication>

#include "global.h"

namespace o3d {
namespace studio {
namespace common {

/**
 * @brief The StoreException class
 */
class O3S_API BaseException : public QException
{
public:

    BaseException(const QString &message);

    void raise() const override;
    BaseException *clone() const override;

    const QString& message() const;

protected:

    QString m_message;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_EXCEPTION_H

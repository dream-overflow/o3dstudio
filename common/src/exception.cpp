/**
 * @brief Common base exceptions
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-04
 * @details
 */

#include "o3d/studio/common/exception.h"

using namespace o3d::studio::common;


BaseException::BaseException(const QString &message) :
    QException(),
    m_message(message)
{

}

void BaseException::raise() const
{
    throw *this;
}

BaseException *BaseException::clone() const
{
    return new BaseException(*this);
}

const QString &BaseException::message() const
{
    return m_message;
}

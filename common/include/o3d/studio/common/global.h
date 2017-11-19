/**
 * @brief Common global
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_GLOBAL_H
#define _O3DS_COMMON_GLOBAL_H

#include <QtCore/QtGlobal>
#include <QtCore/QString>

#include <o3d/core/string.h>

#if defined(MYSHAREDLIB_LIBRARY)
#  define O3S_API Q_DECL_EXPORT
#else
#  define O3S_API Q_DECL_IMPORT
#endif

#ifndef CONV_STRING
#define CONV_STRING

/**
 * @brief Conversion from o3d:String to QString
 * @param str
 * @return
 */
inline QString toQString(const o3d::String &str)
{
    return QString(str.toUtf8().getData());
}

/**
 * @brief Conversion from QString to o3d::String
 * @param str
 * @return
 */
inline o3d::String fromQString(const QString &str)
{
    return o3d::String(str.toStdWString().c_str());
}

#endif // CONV_STRING

#endif // _O3DS_COMMON_GLOBAL_H

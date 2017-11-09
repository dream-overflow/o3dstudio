/**
 * @brief Common global
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-09
 * @details
 */

#ifndef _O3DS_COMMON_GLOBAL_H
#define _O3DS_COMMON_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(MYSHAREDLIB_LIBRARY)
#  define O3S_API Q_DECL_EXPORT
#else
#  define O3S_API Q_DECL_IMPORT
#endif

#endif // _O3DS_COMMON_GLOBAL_H

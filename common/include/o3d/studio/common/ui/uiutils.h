/**
 * @brief Common UI utils
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#ifndef _O3DS_COMMON_UTILS_H
#define _O3DS_COMMON_UTILS_H

#include <QtWidgets/QWidget>
#include <QtGui/QIcon>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API UiUtils
{
public:

    static QIcon tintIcon(const QString &icon, QWidget *widget = nullptr);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_UTILS_H

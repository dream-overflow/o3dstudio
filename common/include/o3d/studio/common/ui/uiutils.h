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

    enum TintMode
    {
        TINT_NORMAL = 0,
        TINT_HIGHLIGHT,
        TINT_DARK,
        TINT_WHITE,
        TINT_NEGATIVE
    };

    // static QIcon tintIcon(const String &icon, QWidget *widget = nullptr, TintMode mode = TINT_NORMAL);
    static QIcon tintIcon(const QString &icon, QWidget *widget = nullptr, TintMode mode = TINT_NORMAL);
    static QIcon tintIcon(QIcon icon, int size = -1, QWidget *widget = nullptr, TintMode mode = TINT_NORMAL);
    static void tintMenu(QList<QAction*> actions, QWidget *widget = nullptr);
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_UTILS_H

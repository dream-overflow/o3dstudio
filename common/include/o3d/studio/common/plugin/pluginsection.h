/**
 * @brief Application plugin section interface
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-28
 * @details
 */

#ifndef _O3DS_COMMON_PLUGINSECTION_H
#define _O3DS_COMMON_PLUGINSECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "../global.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API PluginSection
{
public:

    virtual QString name() const = 0;
    virtual QString label() const = 0;

    // virtual bool addPluginItem(PluginItem *item) = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PLUGINSECTION_H

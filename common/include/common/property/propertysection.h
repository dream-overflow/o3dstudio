/**
 * @brief Application property section interface
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-21
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYSECTION_H
#define _O3DS_COMMON_PROPERTYSECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

namespace o3d {
namespace studio {
namespace common {

class PropertySection
{
public:

    virtual QString name() = 0;
    virtual QString label() = 0;

    virtual bool setupSection(QWidget *parent) = 0;

    virtual void commit() = 0;
    virtual bool save() = 0;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYSECTION_H

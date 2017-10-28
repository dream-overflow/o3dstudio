/**
 * @brief Common module info object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_COMMON_MODULEINFO_H
#define _O3DS_COMMON_MODULEINFO_H

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace o3d {
namespace studio {
namespace common {

class ModuleInfo
{
public:

    ModuleInfo(const QString &name, const QMap<QString, QVariant> &properties);
    ~ModuleInfo();

    const QString& name() const;
    QString verbose() const;
    QString version() const;
    QString vendor() const;
    QString description() const;
    QString section() const;

    QVariant property(const QString &name, const QVariant &defaut = QVariant()) const;

    const QMap<QString, QVariant> properties() const;

protected:

    QMap<QString, QVariant> m_properties;
    QString m_name;               //!< Unique module name
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MODULEINFO_H

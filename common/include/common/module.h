/**
 * @brief Common module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-18
 * @details
 */

#ifndef _O3DS_COMMON_MODULE_H
#define _O3DS_COMMON_MODULE_H

#include <QtCore/QString>
#include <QtCore/QBitArray>

class QLibrary;

#include "capacity.h"
#include "moduleinfo.h"

namespace o3d {
namespace studio {
namespace common {

class Module
{
public:

    Module(const QString &name);
    virtual ~Module();

    const QString& name() const;

    ModuleInfo provideInfo() const;

    virtual QLibrary* library();

    const QBitArray& capacities() const;

    bool hasCapacity(Capacity cap) const;

    virtual bool start();
    virtual bool stop();

    /**
     * @brief Restart the module if loaded, and previously started.
     * @return
     */
    bool restart();

protected:

    QString m_name;                         //!< Unique module name
    QMap<QString, QVariant> m_properties;   //!< Module info and user properties

    QBitArray m_capacities{64};   //!< List of exposed and supported capacities

    bool m_started{false};        //!< Started module capacities
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MODULE_H

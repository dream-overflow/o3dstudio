/**
 * @brief Common dynamic module object specialization
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_COMMON_DYNAMICMODULE_H
#define _O3DS_COMMON_DYNAMICMODULE_H

#include <QtCore/QLibrary>

#include "module.h"

namespace o3d {
namespace studio {
namespace common {

typedef Module* (*PluginFunction)(const QString &, QLibrary *);

class DynamicModule : public Module
{
public:

    DynamicModule(const QString &name, QLibrary *library);
    virtual ~DynamicModule();

    virtual bool start();
    virtual bool stop();

    QString getFileName() const;

    virtual QLibrary* library();

private:

    QLibrary *m_library;           //!< Related library instance
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DYNAMICMODULE_H

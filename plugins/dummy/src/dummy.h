/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_DUMMY_DUMMY_H
#define _O3DS_DUMMY_DUMMY_H

#include <QtCore/QString>
#include "o3d/studio/common/dynamicmodule.h"

namespace o3d {
namespace studio {
namespace dummy {

class Dummy : public studio::common::DynamicModule
{
public:

    Dummy(const QString &name, QLibrary *library);
    virtual ~Dummy();

    virtual bool start();
    virtual bool stop();

private:

};

} // namespace dummy
} // namespace studio
} // namespace o3d

#endif // _O3DS_DUMMY_DUMMY_H

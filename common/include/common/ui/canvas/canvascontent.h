/**
 * @brief Common Qt canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_CANVASCONTENT_H
#define _O3DS_COMMON_CANVASCONTENT_H

#include <QtGui/QImage>
#include "../content.h"

namespace o3d {
namespace studio {
namespace common {

class CanvasContent : public QWidget, public Content
{
    Q_OBJECT

public:

    CanvasContent(QWidget *parent = nullptr);
    virtual ~CanvasContent();
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CANVASCONTENT_H

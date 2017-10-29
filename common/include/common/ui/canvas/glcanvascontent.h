/**
 * @brief Common OpenGL 4.5 canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#ifndef _O3DS_COMMON_GLCANVASCONTENT_H
#define _O3DS_COMMON_GLCANVASCONTENT_H

#include <QtWidgets/QWidget>
#include "../content.h"

namespace o3d {
namespace studio {
namespace common {

class GLCanvasContent : public QWidget, public Content
{
public:

protected:
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_GLCANVASCONTENT_H

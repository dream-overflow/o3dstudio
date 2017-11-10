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
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPixmap>

#include "../content.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API CanvasContent : public QWidget, public Content
{
    Q_OBJECT

public:

    CanvasContent(const QString &name, QWidget *parent = nullptr);
    virtual ~CanvasContent();

    virtual QString elementName() const override;
    virtual QWidget* ui() override;

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

protected:

    virtual void paintEvent(QPaintEvent *event) override;

private:

    bool m_antialiasing;

//    int shape;
//    QPen pen;
//    QBrush brush;
//    bool transformed;
//    QPixmap pixmap;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_CANVASCONTENT_H

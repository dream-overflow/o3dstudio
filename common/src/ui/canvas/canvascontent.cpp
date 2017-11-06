/**
 * @brief Common Qt canvas content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include <QtGui/QPainter>

#include "o3d/studio/common/ui/canvas/canvascontent.h"

using namespace o3d::studio::common;


CanvasContent::CanvasContent(QWidget *parent) :
    QWidget(parent),
    m_antialiasing(true)
{
    setAutoFillBackground(true);

//    shape = 7;
//    transformed = true;
//    pixmap.load(":/images/checkbox.png");
//    pen = QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
//    brush = QBrush(Qt::green, Qt::SolidPattern);
}

CanvasContent::~CanvasContent()
{

}

QString CanvasContent::elementName() const
{
    return "o3s::common::canvascontent";
}

QWidget *CanvasContent::ui()
{
    return this;
}

QSize CanvasContent::sizeHint() const
{
    return QSize(300, 200);
}

QSize CanvasContent::minimumSizeHint() const
{
    return QSize(100, 100);
}

void CanvasContent::paintEvent(QPaintEvent *event)
{
//    static const QPoint points[4] = {
//        QPoint(10, 80),
//        QPoint(20, 10),
//        QPoint(80, 30),
//        QPoint(90, 70)
//    };

//    QRect rect(10, 20, 80, 60);

//    QPainterPath path;
//    path.moveTo(20, 80);
//    path.lineTo(20, 30);
//    path.cubicTo(80, 0, 50, 50, 80, 80);

//    int startAngle = 20 * 16;
//    int arcLength = 120 * 16;

    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width() - 1, height() - 1), QBrush(QColor(255, 255, 255)));

    // painter.setBackground(QBrush(QColor(255, 255, 255)));
    //painter.setBackgroundMode(Qt::OpaqueMode);
//    painter.setPen(pen);
//    painter.setBrush(brush);

    if (m_antialiasing) {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

//    for (int x = 0; x < width(); x += 100) {
//        for (int y = 0; y < height(); y += 100) {
//            painter.save();
//            painter.translate(x, y);

//            if (transformed) {
//                painter.translate(50, 50);
//                painter.rotate(60.0);
//                painter.scale(0.6, 0.9);
//                painter.translate(-50, -50);
//            }

//            switch (shape) {
//                case 0:
//                    painter.drawLine(rect.bottomLeft(), rect.topRight());
//                    break;
//                case 1:
//                    painter.drawPoints(points, 4);
//                    break;
//                case 2:
//                    painter.drawPolyline(points, 4);
//                    break;
//                case 3:
//                    painter.drawPolygon(points, 4);
//                    break;
//                case 4:
//                    painter.drawRect(rect);
//                    break;
//                case 5:
//                    painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
//                    break;
//                case 6:
//                    painter.drawEllipse(rect);
//                    break;
//                case 7:
//                    painter.drawArc(rect, startAngle, arcLength);
//                    break;
//                case 8:
//                    painter.drawChord(rect, startAngle, arcLength);
//                    break;
//                case 9:
//                    painter.drawPie(rect, startAngle, arcLength);
//                    break;
//                case 10:
//                    painter.drawPath(path);
//                    break;
//                case 11:
//                    painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nThe Qt Company"));
//                    break;
//                case 12:
//                    painter.drawPixmap(10, 10, pixmap);
//            }
//            painter.restore();
//        }
//    }

//    painter.setRenderHint(QPainter::Antialiasing, false);
//    painter.setPen(palette().dark().color());
//    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

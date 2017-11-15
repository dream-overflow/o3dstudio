/**
 * @brief Common UI utils
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#include <QtWidgets/QApplication>
#include <QtGui/QPalette>

#include "o3d/studio/common/ui/uiutils.h"

using namespace o3d::studio::common;

QIcon UiUtils::tintIcon(const QString &filename, QWidget *widget)
{
    if (widget == nullptr) {
        widget = QApplication::activeWindow();
    }

    QColor color = QApplication::palette(widget).color(QPalette::Highlight);
    QImage image = QImage(filename);

    // Loop all the pixels
    for(int y = 0; y < image.height(); y++) {
        for(int x= 0; x < image.width(); x++) {
            // Read the alpha value each pixel, keeping the RGB values of your color
            color.setAlpha(image.pixelColor(x, y).alpha());

            // Apply the pixel color
            image.setPixelColor(x, y, color);
        }
    }

    // Get the coloured pixmap
    QPixmap pixmap = QPixmap::fromImage(image);
    return QIcon(pixmap);
}

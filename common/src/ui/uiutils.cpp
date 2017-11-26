/**
 * @brief Common UI utils
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-12
 * @details
 */

#include <QtWidgets/QApplication>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStyle>
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

QIcon UiUtils::tintIcon(QIcon icon, int size, QWidget *widget)
{
    if (widget == nullptr) {
        widget = QApplication::activeWindow();
    }

    if (size < 0) {
        size = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
    }

    QColor color = QApplication::palette(widget).color(QPalette::Highlight);
    QImage image = icon.pixmap(QSize(size, size)).toImage();

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

void UiUtils::tintMenu(QList<QAction*> actions, QWidget *widget)
{
    if (widget == nullptr) {
        widget = QApplication::activeWindow();
    }

    int metric = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);

    // for each actions
    for (QAction *action: actions) {
        if (!action->icon().isNull()) {
            QString name = action->icon().name();

            // ignore list
            if (action->objectName() == "actionAbout") {
                continue;
            }

            if (name.isEmpty()) {
                action->setIcon(tintIcon(action->icon(), metric, widget));
            }
        }

        if (!action->menu()) {
            continue;
        }

        // for each first level
        for (QAction *subAction: action->menu()->actions()) {
            if (!subAction->icon().isNull()) {
                QString name = subAction->icon().name();
                if (name.isEmpty()) {
                    subAction->setIcon(tintIcon(subAction->icon(), metric, widget));
                }
            }

            if (!subAction->menu()) {
                continue;
            }

            // for each sub-level
            for (QAction *subSubAction: subAction->menu()->actions()) {
                if (!subSubAction->icon().isNull()) {
                    QString name = subSubAction->icon().name();
                    if (name.isEmpty()) {
                        subSubAction->setIcon(tintIcon(subAction->icon(), metric, widget));
                    }
                }
            }
        }
    }
}

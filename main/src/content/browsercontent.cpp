/**
 * @brief Application main browser content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-29
 * @details
 */

#include <QtCore/QDir>

#include "browsercontent.h"
#include "common/application.h"

using namespace o3d::studio::main;


BrowserContent:: BrowserContent(QWidget *parent) :
    QTextBrowser(parent)
{
    setSearchPaths(QStringList(common::Application::instance()->appDir() + QDir::separator() + "share/html"));
    setSource(QUrl::fromLocalFile("index.html"));
}

BrowserContent::~BrowserContent()
{

}

QWidget* BrowserContent::ui()
{
    return this;
}

QString BrowserContent::elementName() const
{
    return "o3s::main::browsercontent";
}

QSize BrowserContent::sizeHint() const
{
    return QSize(300, 200);
}

QSize BrowserContent::minimumSizeHint() const
{
    return QSize(100, 100);
}

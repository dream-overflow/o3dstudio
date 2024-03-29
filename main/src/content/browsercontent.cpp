/**
 * @brief Application main browser content
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-29
 * @details
 */

#include <QtCore/QDir>
#include <QtGui/QIcon>

#include "browsercontent.h"
#include "o3d/studio/common/application.h"

using namespace o3d::studio::main;


BrowserContent:: BrowserContent(QWidget *parent) :
    QTextBrowser(parent)
{
    setWindowTitle(tr("Home page"));
    setWindowIcon(QIcon(":/icons/home_black.svg"));

    setSearchPaths(QStringList(toQString(common::Application::instance()->appDir()) + QDir::separator() + "share/html"));
    setSource(QUrl::fromLocalFile("index.html"));
}

BrowserContent::~BrowserContent()
{

}

QWidget* BrowserContent::ui()
{
    return this;
}

o3d::String BrowserContent::elementName() const
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

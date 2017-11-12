/**
 * @brief Application main window main toolbar
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#include "maintoolbar.h"
#include "mainwindow.h"

#include <QtWidgets/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QImage>

#include "o3d/studio/common/command/commandmanager.h"
#include "o3d/studio/common/command/hub/addhubcommand.h"
#include "o3d/studio/common/command/fragment/addfragmentcommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/fragment.h"
#include "o3d/studio/common/workspace/hub.h"
#include "o3d/studio/common/workspace/selection.h"
#include "o3d/studio/common/workspace/selectionitem.h"

using namespace o3d::studio::main;


MainToolBar::MainToolBar(QWidget *parent) :
    QToolBar(tr("Main toolbar"), parent)
{
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::RightToolBarArea);
    setupButtons();

    setWindowIcon(QIcon::fromTheme("applications-accessories"));
}

MainToolBar::~MainToolBar()
{

}

QToolBar *MainToolBar::ui()
{
    return this;
}

QString MainToolBar::elementName() const
{
    return("o3s::main::maintoolbar");
}

Qt::ToolBarArea MainToolBar::toolBarArea() const
{
    return Qt::TopToolBarArea;
}

void MainToolBar::onCreateFragment()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            common::AddFragmentCommand *cmd = new common::AddFragmentCommand(project->ref().light(), QString());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

void MainToolBar::onCreateHub()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace) {
        common::Project *project = workspace->activeProject();
        if (project) {
            common::AddHubCommand *cmd = new common::AddHubCommand(project->ref().light(), QString());
            common::Application::instance()->command().addCommand(cmd);
        }
    }
}

QIcon MainToolBar::tintIcon(const QString &filename)
{
    QColor color = QApplication::palette(this).color(QPalette::Highlight);
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

void MainToolBar::setupButtons()
{
    QAction *addFragment = new QAction(tintIcon(":/icons/fragment_flat.svg"), tr("Create a Fragment"));
    connect(addFragment, SIGNAL(triggered(bool)), SLOT(onCreateFragment()));
    addAction(addFragment);

    QAction *addhub= new QAction(tintIcon(":/icons/device_hub_black.svg"), tr("Create a Hub"));
    connect(addhub, SIGNAL(triggered(bool)), SLOT(onCreateHub()));
    addAction(addhub);

    addSeparator();

    addAction(QIcon::fromTheme("system-search"), tr("Search"));
    addWidget(new QLineEdit());
}

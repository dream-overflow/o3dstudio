/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-20
 * @details
 */

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "displaysection.h"

#include "common/application.h"
#include "common/settings.h"

using namespace o3d::studio::main;


DisplaySection::DisplaySection(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    m_properties["language"] = common::Application::instance()->settings().get("o3s::main::language", QVariant("default")).toString();
    m_properties["themeColor"] = common::Application::instance()->settings().get("o3s::main::theme::color", QVariant("dark")).toString();

    // languages
    ui.language->addItem(tr("Default system"), QVariant("default"));
    ui.language->addItem(tr("English"), QVariant("en"));
    ui.language->addItem(tr("French"), QVariant("fr"));

    // theme colors
    ui.themeColor->addItem(tr("Light theme"), QVariant("light"));
    ui.themeColor->addItem(tr("Dark theme"), QVariant("dark"));
}

DisplaySection::~DisplaySection()
{
}

QString DisplaySection::name()
{
    return "o3s::main::display";
}

QString DisplaySection::label()
{
    return tr("Display");
}

bool DisplaySection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    // languages
    int index = ui.language->findData(QVariant(m_properties["language"]));
    ui.language->setCurrentIndex(index);

    // theme colors
    index = ui.themeColor->findData(QVariant(m_properties["themeColor"]));
    ui.themeColor->setCurrentIndex(index);

    return true;
}

bool DisplaySection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();
}

void DisplaySection::commit()
{
    // @todo store locally data from widgets
    m_properties["language"] = ui.language->currentData().toString();
    m_properties["themeColor"] = ui.themeColor->currentData().toString();

    // event on language and themColor to set active
}

bool DisplaySection::save()
{
    common::Application::instance()->settings().set("o3s::main::language", QVariant(m_properties["language"]));
    common::Application::instance()->settings().set("o3s::main::theme::color", QVariant(m_properties["themeColor"]));

    return true;
}

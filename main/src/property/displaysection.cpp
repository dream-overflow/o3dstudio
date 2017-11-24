/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "displaysection.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/settings.h"

using namespace o3d::studio::main;


DisplaySection::DisplaySection(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    m_language = fromQString(common::Application::instance()->settings().get("o3s::main::language", QVariant("default")).toString());
    m_themeColor = fromQString(common::Application::instance()->settings().get("o3s::main::theme::color", QVariant("darkorange")).toString());

    // languages
    ui.language->addItem(tr("Default system"), QVariant("default"));
    ui.language->addItem(tr("English"), QVariant("en"));
    ui.language->addItem(tr("French"), QVariant("fr"));

    // theme colors
    ui.themeColor->addItem(tr("Default system"), QVariant("default"));
    ui.themeColor->addItem(tr("Dark blue theme"), QVariant("darkblue"));
    ui.themeColor->addItem(tr("Dark orange theme"), QVariant("darkorange"));
}

DisplaySection::~DisplaySection()
{
}

o3d::String DisplaySection::name()
{
    return "o3s::main::display";
}

o3d::String DisplaySection::label()
{
    return fromQString(tr("Display"));
}

o3d::Bool DisplaySection::setupSection(QWidget *parent)
{
    parent->layout()->addWidget(this);
    show();

    // languages
    int index = ui.language->findData(QVariant(toQString(m_language)));
    ui.language->setCurrentIndex(index);

    // theme colors
    index = ui.themeColor->findData(QVariant(toQString(m_themeColor)));
    ui.themeColor->setCurrentIndex(index);

    return True;
}

o3d::Bool DisplaySection::cleanupSection(QWidget *parent)
{
    parent->layout()->removeWidget(this);
    this->setParent(nullptr);

    hide();

    return True;
}

void DisplaySection::commit()
{
    // @todo store locally data from widgets
    m_language = fromQString(ui.language->currentData().toString());
    m_themeColor = fromQString(ui.themeColor->currentData().toString());

    // event on language and themeColor to set active
}

o3d::Bool DisplaySection::save()
{
    common::Application::instance()->settings().set("o3s::main::language", QVariant(toQString(m_language)));
    common::Application::instance()->settings().set("o3s::main::theme::color", QVariant(toQString(m_themeColor)));

    return True;
}

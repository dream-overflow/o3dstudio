/**
 * @brief Application display properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-20
 * @details
 */

#ifndef _O3DS_MAIN_DISPLAYSECTION_H
#define _O3DS_MAIN_DISPLAYSECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "o3d/studio/common/property/propertysection.h"

#include "ui_displaysection.h"

namespace o3d {
namespace studio {
namespace main {

class DisplaySection : public QWidget, public common::PropertySection
{
    Q_OBJECT

public:

    DisplaySection(QWidget *parent = 0);
    virtual ~DisplaySection();

    virtual String name() override;
    virtual String label() override;

    virtual Bool setupSection(QWidget *parent) override;
    virtual Bool cleanupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual Bool save() override;

private:

    Ui::DisplaySection ui;

    String m_name;
    String m_label;

    String m_language;
    String m_themeColor;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_DISPLAYSECTION_H

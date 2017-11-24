/**
 * @brief Application project properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-08
 * @details
 */

#ifndef _O3DS_MAIN_PROJECTSECTION_H
#define _O3DS_MAIN_PROJECTSECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QtWidgets>

#include "o3d/studio/common/property/propertysection.h"
#include "o3d/studio/common/objectref.h"

// #include "ui_projectsection.h"

namespace o3d {
namespace studio {
namespace main {

class ProjectSection : public QWidget, public common::PropertySection
{
    Q_OBJECT

public:

    ProjectSection(const common::LightRef &ref, QWidget *parent = 0);
    virtual ~ProjectSection();

    virtual String name() override;
    virtual String label() override;

    virtual Bool setupSection(QWidget *parent) override;
    virtual Bool cleanupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual Bool save() override;

private:

    // Ui::WorkspaceSection ui;

    String m_name;
    String m_label;

    common::LightRef m_ref;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PROJECTSECTION_H

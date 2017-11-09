/**
 * @brief Application project properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-08
 * @details
 */

#ifndef _O3DS_MAIN_PROJECTSECTION_H
#define _O3DS_MAIN_PROJECTSECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
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

    virtual QString name() override;
    virtual QString label() override;

    virtual bool setupSection(QWidget *parent) override;
    virtual bool cleanupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual bool save() override;

private:

    // Ui::WorkspaceSection ui;

    QMap<QString, QVariant> m_properties;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PROJECTSECTION_H

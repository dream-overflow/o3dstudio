/**
 * @brief Application active workspace properties section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-08
 * @details
 */

#ifndef _O3DS_MAIN_WORKSPACESECTION_H
#define _O3DS_MAIN_WORKSPACESECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include <o3d/core/uuid.h>

#include "o3d/studio/common/property/propertysection.h"

// #include "ui_workspacesection.h"

namespace o3d {
namespace studio {
namespace main {

class WorkspaceSection : public QWidget, public common::PropertySection
{
    Q_OBJECT

public:

    WorkspaceSection(const Uuid &ref, QWidget *parent = 0);
    virtual ~WorkspaceSection();

    virtual String name() override;
    virtual String label() override;

    virtual Bool setupSection(QWidget *parent) override;
    virtual Bool cleanupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual Bool save() override;

private:

    // Ui::WorkspaceSection ui;

    std::map<String, QVariant> m_properties;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_WORKSPACESECTION_H

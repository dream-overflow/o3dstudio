/**
 * @brief Application common transform dock related to current transform manipulator
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-04-22
 * @details
 */

#ifndef _O3DS_MAIN_TRANSFORMDOCK_H
#define _O3DS_MAIN_TRANSFORMDOCK_H

class QListWidget;

#include <QtWidgets/QDockWidget>
#include <QtCore/QCoreApplication>

#include <o3d/core/baseobject.h>
#include <o3d/core/templatebitset.h>

#include "o3d/studio/common/ui/dock.h"
#include "o3d/studio/common/objectref.h"
#include "o3d/studio/common/ui/panel.h"

namespace o3d {
namespace studio {
namespace common {

class Vector3Property;

/**
 * @brief The TransformPanel
 */
class TransformPanel : public Panel, public EvtHandler
{
    Q_DECLARE_TR_FUNCTIONS(TransformPanel)

public:

    TransformPanel();
    virtual ~TransformPanel();

    virtual String elementName() const override;

    virtual QWidget* ui() override;

    virtual PanelType panelType() const override;

    virtual void commit() override;
    virtual void update() override;

private:

    // @todo multi transforms
    Vector3Property *m_position;
    Vector3Property *m_rotation;
    Vector3Property *m_scale;

    Int32 m_manualMode;
};

/**
 * @brief The TransformDock display the actual manipulator transformation.
 */
class TransformDock : public BaseObject, public Dock
{
    Q_DECLARE_TR_FUNCTIONS(TransformDock)

public:

    TransformDock(BaseObject *parent = nullptr);
    virtual ~TransformDock();

    virtual QDockWidget* ui() override;
    virtual String elementName() const override;
    virtual Qt::DockWidgetArea dockWidgetArea() const override;

public /*slots*/:

    void onSelectionChanged();

    void onChangeCurrentWorkspace(const String &name);
    void onProjectEntityChanged(LightRef ref, BitSet64 changeFlags);

    void onActivateProject(LightRef ref);
    void onActivateFragment(LightRef ref);

private:

    class QtTransformDock *m_qtTransformDock;
    class TransformPanel *m_panel;

    void setupPanel();
    void cleanupPanel();
};

class QtTransformDock : public QDockWidget
{
    Q_OBJECT

    friend class TransformDock;

public:

    QtTransformDock(QWidget *parent = nullptr);
    virtual ~QtTransformDock();

    class QLayout *container();

    void setupUi();

signals:

public slots:

public:

    class QWidget *m_container;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_TRANSFORMDOCK_H

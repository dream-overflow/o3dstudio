/**
 * @brief Workspace current workspace current selection management.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-11-08
 * @details
 */

#ifndef _O3DS_COMMON_SELECTION_H
#define _O3DS_COMMON_SELECTION_H

#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtCore/QSet>
#include <QtCore/QMap>

#include "workspace.h"
#include "selectionitem.h"

namespace o3d {
namespace studio {
namespace common {

class Hub;

class O3S_API Selection : public QObject
{
    Q_OBJECT

public:

    Selection(QObject *parent = nullptr);
    virtual ~Selection();

    /**
     * @brief initialize with a different workspace
     * @param workspace
     */
    void initialize(Workspace *workspace);

    /**
     * @brief terminate Cleanup for current workspace
     */
    void terminate();

    /**
     * @brief begin a multiple selection that will be validated and thrown at endSelection call.
     */
    void beginSelection();

    void appendSelection(Entity *entity);

    /**
     * @brief endSelection Validate the selection transaction and emit signal.
     */
    void endSelection();

    /**
     * @brief select a single entity (project, fragment, hub) and emit signal.
     * @param project
     */
    void select(Entity *entity);

    /**
     * @brief unselect all current and emit signal.
     */
    void unselectAll();

    const QSet<SelectionItem*> previousSelection() const;
    const QSet<SelectionItem*> currentSelection() const;

    const QSet<SelectionItem*> filterPrevious(qint64 type) const;
    const QSet<SelectionItem*> filterCurrent(qint64 type) const;

public slots:

signals:

    void selectionChanged();

private slots:

private:

    bool m_selecting;

    //! Related workspace
    Workspace *m_workspace;

    //! Current set of selected items
    QSet<SelectionItem*> m_currentSelection;
    //! Previous set of selected items
    QSet<SelectionItem*> m_previousSelection;

    //! Current set of selected items
    QSet<LightRef> m_selectingSet;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTION_H

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
#include <QtCore/QSet>

#include "workspace.h"
#include "selectionitem.h"

namespace o3d {
namespace studio {
namespace common {

class Selection : public QObject
{
    Q_OBJECT

public:

    Selection(QObject *parent = nullptr);
    virtual ~Selection();

    void initialize(Workspace *workspace);
    void terminate();

    void select(Project *project);
    // void select(Hub *hub);

    void unselectAll();

    const QSet<SelectionItem*> previousSelection() const;
    const QSet<SelectionItem*> currentSelection() const;

    const QSet<SelectionItem*> filterPrevious(SelectionItem::SelectionType type) const;
    const QSet<SelectionItem*> filterCurrent(SelectionItem::SelectionType type) const;

public slots:


signals:

    void selectionChanged();

private:

    //! Related workspace
    Workspace *m_workspace;

    //! Current set of selected items
    QSet<SelectionItem*> m_currentSelection;
    //! Previous set of selected items
    QSet<SelectionItem*> m_previousSelection;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SELECTION_H

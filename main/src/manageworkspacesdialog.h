/**
 * @brief Manage workspaces dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_MANAGEWORKSPACESDIALOG_H
#define _O3DS_MAIN_MANAGEWORKSPACESDIALOG_H

#include <QtWidgets/QDialog>

#include "ui_manageworkspacesdialog.h"

namespace o3d {
namespace studio {
namespace main {

class ManageWorkspaceDialog : public QDialog
{
    Q_OBJECT

public:

    ManageWorkspaceDialog(QWidget *parent = nullptr);
    virtual ~ManageWorkspaceDialog();

protected:

    virtual void closeEvent(QCloseEvent*);

private slots:

    // void onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous);
    void onButtonBox(QAbstractButton *);

private:

    Ui::ManageWorkspacesDialog ui;

    void setupWorkspaces();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_MANAGEWORKSPACESDIALOG_H

/**
 * @brief New project dialog
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-23
 * @details
 */

#ifndef _O3DS_MAIN_NEWPROJECTDIALOG_H
#define _O3DS_MAIN_NEWPROJECTDIALOG_H

#include <QtWidgets/QDialog>

#include "ui_newprojectdialog.h"

namespace o3d {
namespace studio {
namespace main {

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:

    NewProjectDialog(QWidget *parent = nullptr);
    virtual ~NewProjectDialog();

protected:

    virtual void closeEvent(QCloseEvent*);

private slots:

    void onButtonBox(QAbstractButton *);

private:

    Ui::NewProjectDialog ui;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_NEWPROJECTDIALOG_H

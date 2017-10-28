/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_MAIN_PREFERENCESDIALOG_H
#define _O3DS_MAIN_PREFERENCESDIALOG_H

#include <QtWidgets/QDialog>

#include "ui_preferencesdialog.h"

namespace o3d {
namespace studio {
namespace main {

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:

    PreferencesDialog(QWidget *parent = nullptr);
    virtual ~PreferencesDialog();

protected:

    virtual void closeEvent(QCloseEvent*);
    virtual void changeEvent(QEvent*);

private slots:

    void onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous);
    void onButtonBox(QAbstractButton *);

private:

    Ui::PreferencesDialog ui;

    void setupCategories();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PREFERENCESDIALOG_H

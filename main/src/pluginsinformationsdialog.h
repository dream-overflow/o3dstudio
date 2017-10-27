/**
 * @brief dummy module object
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-19
 * @details
 */

#ifndef _O3DS_MAIN_PLUGINSINFORMATIONSDIALOG_H
#define _O3DS_MAIN_PLUGINSINFORMATIONSDIALOG_H

#include <QtWidgets/QDialog>

#include "ui_pluginsinformationsdialog.h"

namespace o3d {
namespace studio {
namespace main {

class PluginsInformationsDialog : public QDialog
{
    Q_OBJECT

public:

    PluginsInformationsDialog(QWidget *parent = nullptr);
    virtual ~PluginsInformationsDialog();

protected:

    virtual void closeEvent(QCloseEvent*);

private slots:

  //  void onSectionPropertyChanged(const QModelIndex &current, const QModelIndex &previous);
  //  void onButtonBox(QAbstractButton *);

private:

    Ui::PluginsInformationsDialog ui;

    // void setupCategories();
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PLUGINSINFORMATIONSDIALOG_H

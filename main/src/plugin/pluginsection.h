/**
 * @brief Application display plugin section
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-28
 * @details
 */

#ifndef _O3DS_MAIN_PLUGINSECTION_H
#define _O3DS_MAIN_PLUGINSECTION_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "o3d/studio/common/plugin/pluginsection.h"

namespace o3d {
namespace studio {
namespace main {

class PluginSection : public QWidget , public common::PluginSection
{
    Q_OBJECT

public:

    PluginSection(const QString &name, const QString &label, QWidget *parent = 0);
    virtual ~PluginSection();

    virtual QString name() const override;
    virtual QString label() const override;

private:

    QMap<QString, QVariant> m_properties;

    QString m_name;
    QString m_label;
};

} // namespace main
} // namespace studio
} // namespace o3d

#endif // _O3DS_MAIN_PLUGINSECTION_H

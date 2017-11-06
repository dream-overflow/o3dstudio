/**
 * @brief Application generic plugin section node
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-21
 * @details
 */

#ifndef _O3DS_COMMON_PLUGINSECTIONNODE_H
#define _O3DS_COMMON_PLUGINSECTIONNODE_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "pluginsection.h"

namespace o3d {
namespace studio {
namespace common {

class PluginSectionNode : public QWidget , public PluginSection
{
    Q_OBJECT

public:

    PluginSectionNode(const QString &section, const QString &label, QWidget *parent = 0);
    virtual ~PluginSectionNode();

    virtual QString name() const override;
    virtual QString label() const override;

protected:

    QString m_name;
    QString m_label;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PLUGINSECTIONNODE_H

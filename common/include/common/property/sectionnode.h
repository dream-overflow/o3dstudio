/**
 * @brief Application generic section node
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@gmail.com)
 * @date 2017-10-21
 * @details
 */

#ifndef _O3DS_COMMON_SECTIONNODE_H
#define _O3DS_COMMON_SECTIONNODE_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtWidgets/QtWidgets>

#include "propertysection.h"

namespace o3d {
namespace studio {
namespace common {

class SectionNode : public QWidget , public PropertySection
{
    Q_OBJECT

public:

    SectionNode(const QString &section, const QString &label, QWidget *parent = 0);
    virtual ~SectionNode();

    virtual QString name() override;
    virtual QString label() override;

    virtual bool setupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual bool save() override;

protected:

    QString m_name;
    QString m_label;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SECTIONNODE_H

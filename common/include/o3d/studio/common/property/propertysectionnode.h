/**
 * @brief Application generic property section node
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-10-21
 * @details
 */

#ifndef _O3DS_COMMON_PROPERTYSECTIONNODE_H
#define _O3DS_COMMON_PROPERTYSECTIONNODE_H

#include "propertysection.h"

namespace o3d {
namespace studio {
namespace common {

class O3S_API PropertySectionNode : public PropertySection
{
public:

    PropertySectionNode(const String &section, const String &label);
    virtual ~PropertySectionNode();

    virtual String name() override;
    virtual String label() override;

    virtual Bool setupSection(QWidget *parent) override;
    virtual Bool cleanupSection(QWidget *parent) override;

    virtual void commit() override;
    virtual Bool save() override;

protected:

    String m_name;
    String m_label;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_PROPERTYSECTIONNODE_H

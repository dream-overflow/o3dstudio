/**
 * @brief Common viewport component offering a viewport hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#ifndef _O3DS_COMMON_VIEWPORTHUB_H
#define _O3DS_COMMON_VIEWPORTHUB_H

#include "component.h"
#include "../workspace/hub.h"

namespace o3d {
namespace studio {
namespace common {

class Entity;
class Project;

/**
 * @brief The ViewportComponent class
 */
class O3S_API ViewportComponent : public Component
{
public:

    ViewportComponent();
    virtual ~ViewportComponent();

    virtual void setup() override;
    virtual Entity* instanciate(const QString &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The ViewportHub class
 */
class O3S_API ViewportHub : public Hub
{
public:

    explicit ViewportHub(const QString &name, Entity *parent = nullptr);
    virtual ~ViewportHub();

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

private:

    QPointF m_pos;
    QSizeF m_size;

    bool m_sizeInPercent;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_VIEWPORTHUB_H

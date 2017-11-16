/**
 * @brief Common duumy component offering a dummy hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-16
 * @details
 */

#ifndef _O3DS_COMMON_DUMMYHUB_H
#define _O3DS_COMMON_DUMMYHUB_H

#include "component.h"
#include "../workspace/hub.h"

namespace o3d {
namespace studio {
namespace common {

class Project;
class Hub;

/**
 * @brief The DummyHubComponent class
 */
class O3S_API DummyHubComponent : public Component
{
public:

    DummyHubComponent();
    virtual ~DummyHubComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const QString &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The DummyHub class
 */
class O3S_API DummyHub : public Hub
{
public:

    explicit DummyHub(const QString &name, Entity *parent = nullptr);
    virtual ~DummyHub();

    virtual void create() override;

    virtual bool load() override;
    virtual bool save() override;

    virtual bool exists() const override;

    virtual bool serializeContent(QDataStream &stream) const;
    virtual bool deserializeContent(QDataStream &stream);

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

protected:
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DUMMYHUB_H

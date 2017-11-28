/**
 * @brief Common dummy component offering a dummy hub.
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
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The DummyHub class
 */
class O3S_API DummyHub : public Hub
{
public:

    explicit DummyHub(const String &name, Entity *parent = nullptr);
    virtual ~DummyHub();

    virtual void create() override;
    virtual Bool deletable() const override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const;
    virtual Bool deserializeContent(InStream &stream);

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

protected:
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DUMMYHUB_H

/**
 * @brief Common drawer component offering a drawer hub.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-17
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
 * @brief The DrawerHubComponent class
 */
class O3S_API DrawerHubComponent : public Component
{
public:

    DrawerHubComponent();
    virtual ~DrawerHubComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The DrawerHub class
 */
class O3S_API DrawerHub : public Hub
{
public:

    enum DrawerType
    {
        DRAW_FORWARD_RENDERING = 0,
        DRAW_FORWARD_RENDERING_SHADOW_VOLUME = 1,
        DRAW_DEFERED_RENDERING = 2
    };

    explicit DrawerHub(const String &name, Entity *parent = nullptr);
    virtual ~DrawerHub();

    virtual void create() override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const;
    virtual Bool deserializeContent(InStream &stream);

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

    //
    // Properties
    //

    // @todo

protected:
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_DUMMYHUB_H

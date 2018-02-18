/**
 * @brief Common light component offering a light hub.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-18
 * @details
 */

#ifndef _O3DS_COMMON_LIGHTHUB_H
#define _O3DS_COMMON_LIGHTHUB_H

#include "component.h"
#include "../workspace/hub.h"

#include <map>

namespace o3d {

class Light;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

/**
 * @brief The LightComponent class
 */
class O3S_API LightHubComponent : public Component
{
public:

    LightHubComponent();
    virtual ~LightHubComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

protected:
};

/**
 * @brief The LightHub class
 */
class O3S_API LightHub : public Hub
{
public:

    /**
     * Type of the light.
     */
    enum LightType
    {
        POINT_LIGHT = 0,     //!< Positional point light (all directions).
        SPOT_LIGHT,          //!< Positional and directional spot light with cutoff.
        DIRECTIONAL_LIGHT,   //!< Directional infinite light.
        LIGHT_MAP            //!< Light map.
    };

    explicit LightHub(const String &name, Entity *parent = nullptr);
    virtual ~LightHub();

    virtual void create() override;
    virtual void destroy() override;

    virtual Bool deletable() const override;

    virtual Bool load() override;
    virtual Bool save() override;

    virtual Bool exists() const override;

    virtual Bool serializeContent(OutStream &stream) const override;
    virtual Bool deserializeContent(InStream &stream) override;

    virtual void createToScene(MasterScene *masterScene) override;
    virtual void removeFromScene(MasterScene *masterScene) override;
    virtual void syncWithScene(MasterScene *masterScene) override;

    //
    // Properties
    //

    // @todo

protected:

    LightType m_lightType;

    std::map<MasterScene*, o3d::Light*> m_instances;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_LIGHTHUB_H

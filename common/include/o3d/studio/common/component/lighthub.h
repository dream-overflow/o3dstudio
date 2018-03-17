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

#include <o3d/image/color.h>

#include <map>

namespace o3d {

class Light;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;
class Vector3Property;
class QuadraticProperty;
class ColorProperty;
class DropDownProperty;
class FloatProperty;

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
    virtual Panel* panel(Panel::PanelType panelType, Hub *hub) override;

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
    virtual void update() override;

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

    void setLightType(LightType type);
    void setAttenuation(const Vector3& quadratic);
    void setAmbient(const Color& color);
    void setDiffuse(const Color& color);
    void setSpecular(const Color& color);
    void setExponent(Float exp);
    void setCutOff(Float angle);

    LightType lightType() const;
    const Vector3& attenuation() const;
    const Color& ambient() const;
    const Color& diffuse() const;
    const Color& specular() const;
    Float exponent() const;
    Float cutOff() const;

protected:

    LightType m_lightType;
    Vector3 m_attenuation;
    Color m_ambient;
    Color m_diffuse;
    Color m_specular;
    Float m_exponent;
    Float m_cutOff;

    std::map<MasterScene*, o3d::Light*> m_instances;
};

/**
 * @brief The LightPropertyPanel
 */
class LightPropertyPanel : public Panel, public EvtHandler
{
    Q_DECLARE_TR_FUNCTIONS(LightPropertyPanel)

public:

    LightPropertyPanel(LightHub *hub);
    virtual ~LightPropertyPanel();

    virtual String elementName() const override;

    virtual QWidget* ui() override;

    virtual PanelType panelType() const override;

    virtual void commit() override;
    virtual void update() override;

public /*slot*/:

    void onLightTypeChanged(Int32 lightType);

private:

    LightRef m_ref;
    LightHub *m_hub;

    // @todo multi transforms
    DropDownProperty *m_lightType;
    QuadraticProperty *m_attenuation;
    ColorProperty *m_ambient;
    ColorProperty *m_diffuse;
    ColorProperty *m_specular;
    FloatProperty *m_exponent;
    FloatProperty *m_cutOff;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_LIGHTHUB_H

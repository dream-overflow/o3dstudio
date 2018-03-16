/**
 * @brief Common light component offering a light hub.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/object/light.h>

#include "o3d/studio/common/component/lighthub.h"
#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include "o3d/studio/common/ui/panelbuilder.h"
#include "o3d/studio/common/ui/property/vector3property.h"
#include "o3d/studio/common/ui/property/colorproperty.h"
#include "o3d/studio/common/ui/property/floatproperty.h"
#include "o3d/studio/common/ui/property/dropdownproperty.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

using namespace o3d::studio::common;


LightHubComponent::LightHubComponent()
{
    m_name = "o3s::common::component::lighthub";
    m_targetName = "o3s::common::hub::light";

    m_label = "Light Hub";
    m_icon = ":/icons/wb_incandescent_black.svg";
    // m_icon = ":/icons/wb_sunny_black.svg";
}

LightHubComponent::~LightHubComponent()
{

}

void LightHubComponent::setup()
{

}

Hub *LightHubComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    LightHub *lightHub = new LightHub(name, parent);
    lightHub->setTypeRef(m_targetTypeRef);

    return lightHub;
}

Panel *LightHubComponent::panel(Panel::PanelType panelType, Hub *hub)
{
    if (!hub) {
        return nullptr;
    }

    if (hub->ref().strong().typeName() != m_targetName) {
        // not a light hub
        return nullptr;
    }

    LightHub *lhub = static_cast<LightHub*>(hub);

    if (panelType == Panel::PANEL_PROPERTY) {
        return new LightPropertyPanel(lhub);
    }

    return nullptr;
}

LightHub::LightHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_lightType(POINT_LIGHT),
    m_attenuation(1.0f, 0.0f, 0.0f),
    m_ambient(0.7f, 0.7f, 0.7f, 1.0f),
    m_diffuse(0.7f, 0.7f, 0.7f, 1.0f),
    m_specular(0.7f ,0.7f, 0.7f ,1.0f),
    m_exponent(0.0f),
    m_cutOff(90.f),
    m_instances()
{

}

LightHub::~LightHub()
{
    O3D_ASSERT(m_instances.empty());
}

void LightHub::create()
{
    Hub::create();

     // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void LightHub::destroy()
{
    Hub::destroy();

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

void LightHub::update()
{
     for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::SYNC);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool LightHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool LightHub::load()
{
    return Hub::load();
}

o3d::Bool LightHub::save()
{
    return Hub::save();
}

o3d::Bool LightHub::exists() const
{
    return Entity::exists();
}

o3d::Bool LightHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool LightHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void LightHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Light *light = new o3d::Light(masterScene->scene());
    light->setName(m_name);
    light->enableVisibility();

    if (m_lightType == POINT_LIGHT) {
        light->setLightType(o3d::Light::POINT_LIGHT);
    } else if (m_lightType == SPOT_LIGHT) {
        light->setLightType(o3d::Light::SPOT_LIGHT);
    } else if (m_lightType == DIRECTIONAL_LIGHT) {
        light->setLightType(o3d::Light::DIRECTIONAL_LIGHT);
    } else if (m_lightType == LIGHT_MAP) {
        light->setLightType(o3d::Light::LIGHT_MAP);
    }

    light->setAmbient(m_ambient);
    light->setDiffuse(m_diffuse);
    light->setSpecular(m_specular);
    light->setExponent(m_exponent);
    light->setAttenuation(m_attenuation);
    light->setCutOff(m_cutOff);

    // if the parent hub is a spacial node add the light the it
    if (parent() && parent()->isParentHub() && static_cast<Hub*>(parent())->isSpacialNode()) {
        SpacialNodeHub *parentHub = static_cast<SpacialNodeHub*>(parent());
        parentHub->addChildToScene(masterScene, light);
    }

    m_instances[masterScene] = light;

    // scene object id is as the base of the pickable color id
    project()->addPickable((UInt32)light->getId(), this);

    O3D_MESSAGE("LightHub created into scene");
}

void LightHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Light *light = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)light->getId());

        light->getParent()->deleteChild(light);

        O3D_MESSAGE("LightHub deleted from scene");
    }
}

void LightHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Light *light = it->second;

        // hub => o3d
        light->setName(m_name);

        if (m_lightType == POINT_LIGHT) {
            light->setLightType(o3d::Light::POINT_LIGHT);
        } else if (m_lightType == SPOT_LIGHT) {
            light->setLightType(o3d::Light::SPOT_LIGHT);
        } else if (m_lightType == DIRECTIONAL_LIGHT) {
            light->setLightType(o3d::Light::DIRECTIONAL_LIGHT);
        } else if (m_lightType == LIGHT_MAP) {
            light->setLightType(o3d::Light::LIGHT_MAP);
        }

        light->setAmbient(m_ambient);
        light->setDiffuse(m_diffuse);
        light->setSpecular(m_specular);
        light->setExponent(m_exponent);
        light->setAttenuation(m_attenuation);
        light->setCutOff(m_cutOff);

        // o3d => hub

        // nothing to do

        O3D_MESSAGE("LightHub synced into scene");
    }
}

void LightHub::setLightType(LightHub::LightType type)
{
    m_lightType = type;
}

void LightHub::setAttenuation(const o3d::Vector3 &quadratic)
{
    m_attenuation = quadratic;
}

void LightHub::setAmbient(const o3d::Color &color)
{
    m_ambient = color;
}

void LightHub::setDiffuse(const o3d::Color &color)
{
    m_diffuse = color;
}

void LightHub::setSpecular(const o3d::Color &color)
{
    m_specular = color;
}

void LightHub::setExponent(o3d::Float exp)
{
    m_exponent = exp;
}

void LightHub::setCutOff(o3d::Float angle)
{
    m_cutOff = angle;
}

LightHub::LightType LightHub::lightType() const
{
    return m_lightType;
}

const o3d::Vector3 &LightHub::attenuation() const
{
    return m_attenuation;
}

const o3d::Color &LightHub::ambient() const
{
    return m_ambient;
}

const o3d::Color &LightHub::diffuse() const
{
    return m_diffuse;
}

const o3d::Color &LightHub::specular() const
{
    return m_specular;
}

o3d::Float LightHub::exponent() const
{
    return m_exponent;
}

o3d::Float LightHub::cutOff() const
{
    return m_cutOff;
}

LightPropertyPanel::LightPropertyPanel(LightHub *hub) :
    m_hub(hub)
{
    O3D_ASSERT(m_hub);
    O3D_ASSERT(m_hub->exists());

    if (m_hub) {
        m_ref = m_hub->ref().light();
    }
}

LightPropertyPanel::~LightPropertyPanel()
{

}

o3d::String LightPropertyPanel::elementName() const
{
    return "o3s::common::component::panel::property::light";
}

QWidget *LightPropertyPanel::ui()
{
    PanelBuilder pb(this, fromQString(tr("Light hub")));

    m_lightType = new DropDownProperty(this, "lightType", fromQString(tr("Type")));
    pb.addPanelProperty(m_lightType);

    m_lightType->addValue(fromQString(tr("Point light")), LightHub::POINT_LIGHT);
    m_lightType->addValue(fromQString(tr("Spot light")), LightHub::SPOT_LIGHT);
    m_lightType->addValue(fromQString(tr("Directional light")), LightHub::DIRECTIONAL_LIGHT);
    m_lightType->addValue(fromQString(tr("Light-map")), LightHub::LIGHT_MAP);

    m_lightType->onValueChanged.connect(this, &LightPropertyPanel::onLightTypeChanged);

    m_lightType->onValueChanged.connect(this, [this] (Int32) {
        commit();
    });

    m_attenuation = new Vector3Property(this, "attenuation", fromQString(tr("Quadratic attenuation")));
    m_attenuation->setMinMax(0, 1000000);
    pb.addPanelProperty(m_attenuation);

    m_attenuation->onValueChanged.connect(this, [this] (Vector3) {
        commit();
    });

    m_exponent = new FloatProperty(this, "exponent", fromQString(tr("Exponent")));
    pb.addPanelProperty(m_exponent);

    m_exponent->onValueChanged.connect(this, [this] (Float) {
        commit();
    });

    // contextual if spot light
    m_cutOff = new FloatProperty(this, "cutoff", fromQString(tr("Cut-off")));
    pb.addPanelProperty(m_cutOff);

    m_cutOff->onValueChanged.connect(this, [this] (Float) {
        commit();
    });

    m_ambient = new ColorProperty(this, "ambient", fromQString(tr("Ambient color")), ColorProperty::COLOR_RGB);
    pb.addPanelProperty(m_ambient);

    m_ambient->onValueChanged.connect(this, [this] (Color) {
        commit();
    });

    m_diffuse = new ColorProperty(this, "diffuse", fromQString(tr("Diffuse color")), ColorProperty::COLOR_RGB);
    pb.addPanelProperty(m_diffuse);

    m_diffuse->onValueChanged.connect(this, [this] (Color) {
        commit();
    });

    m_specular = new ColorProperty(this, "specular", fromQString(tr("Specular color")), ColorProperty::COLOR_RGB);
    pb.addPanelProperty(m_specular);

    m_specular->onValueChanged.connect(this, [this] (Color) {
        commit();
    });

    return pb.ui();
}

Panel::PanelType LightPropertyPanel::panelType() const
{
    return PANEL_PROPERTY;
}

void LightPropertyPanel::commit()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace->hub(m_ref) == m_hub) {
        m_hub->setLightType((LightHub::LightType)m_lightType->value());
        m_hub->setAmbient(m_ambient->value());
        m_hub->setDiffuse(m_diffuse->value());
        m_hub->setSpecular(m_specular->value());
        m_hub->setExponent(m_exponent->value());
        m_hub->setAttenuation(m_attenuation->value());
        m_hub->setCutOff(m_cutOff->value());

        m_hub->update();
    }
}

void LightPropertyPanel::update()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace->hub(m_ref) == m_hub) {
        m_lightType->setValue(m_hub->lightType());
        m_ambient->setValue(m_hub->ambient());
        m_diffuse->setValue(m_hub->diffuse());
        m_specular->setValue(m_hub->specular());
        m_exponent->setValue(m_hub->exponent());
        m_attenuation->setValue(m_hub->attenuation());
        m_cutOff->setValue(m_hub->cutOff());
    }
}

void LightPropertyPanel::onLightTypeChanged(o3d::Int32 lightType)
{
    // display/hide cutoff
    if (lightType == LightHub::SPOT_LIGHT) {
        m_cutOff->show();
    } else {
        m_cutOff->hide();
    }
}

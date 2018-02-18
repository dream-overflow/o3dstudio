/**
 * @brief Common bones of skeleton component offering a 3d bones for skeleton.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_BONESHUB_H
#define _O3DS_COMMON_BONESHUB_H

#include "component.h"
#include "../workspace/hub.h"

#include <o3d/core/vector3.h>

#include <map>

namespace o3d {

class Bones;
class SceneObject;
class Transform;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;
class BoolProperty;
class FloatProperty;
class Vector3Property;

/**
 * @brief The BonesComponent class
 */
class O3S_API BonesComponent : public Component
{
public:

    BonesComponent();
    virtual ~BonesComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

    virtual SceneUIElement *sceneUiElement(SceneUIElement::Mode mode) override;
    virtual Panel* panel(Panel::PanelType panelType, Hub *hub) override;

protected:
};

/**
 * @brief The BonesHub class
 */
class O3S_API BonesHub : public Hub
{
public:

    explicit BonesHub(const String &name, Entity *parent = nullptr);
    virtual ~BonesHub();

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
    // Transformations
    //

    // assume one transform for the moment
    void setPosition(const o3d::Vector3f &pos);
    void setRotation(const o3d::Vector3f &rot);
    void setScale(const o3d::Vector3f &scale);

    //
    // Properties
    //

    // @todo manage skeleton, children and animations

    /**
     * @brief Add the scene object to the bones of the corresponding master scene.
     * @param masterScene Target master scene.
     * @param sceneObject Valid scene object to setup into.
     * @return True if master scene exists, and added into it.
     */
    virtual Bool addChildToScene(MasterScene *masterScene, SceneObject *sceneObject);

    /**
     * @brief Remove the scene object to the bones of the corresponding master scene.
     * @param masterScene Target master scene.
     * @param sceneObject Valid scene object to remove from (not delete).
     * @return True if master scene exists, object found and removed from it.
     */
    virtual Bool removeChildFromScene(MasterScene *masterScene, SceneObject *sceneObject);

protected:

    std::map<MasterScene*, o3d::Bones*> m_instances;

    Bool m_isEndEffector;

    Float m_length;
    Float m_radius;

    o3d::Transform* m_transform;
};

/**
 * @brief The BonesPropertyPanel
 */
class BonesPropertyPanel : public Panel
{
    Q_DECLARE_TR_FUNCTIONS(BonesPropertyPanel)

public:

    BonesPropertyPanel(BonesHub *hub);
    virtual ~BonesPropertyPanel();

    virtual String elementName() const override;

    virtual QWidget* ui() override;

    virtual PanelType panelType() const override;

    virtual void commit() override;
    virtual void update() override;

private:

    BonesHub *m_hub;

    BoolProperty *m_isEndEffector;

    FloatProperty *m_length;
    FloatProperty *m_radius;

    Vector3Property *m_position;
    Vector3Property *m_rotation;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_BONESHUB_H

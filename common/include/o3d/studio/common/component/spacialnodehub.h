/**
 * @brief Common spacial node component offering a 3d node for scene graph.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-08
 * @details
 */

#ifndef _O3DS_COMMON_SPACIALNODEHUB_H
#define _O3DS_COMMON_SPACIALNODEHUB_H

#include "component.h"
#include "../workspace/structuralhub.h"

#include <o3d/core/vector3.h>
#include <o3d/core/quaternion.h>

#include <map>

namespace o3d {

class Node;
class SceneObject;
class Transform;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;
class Vector3Property;

/**
 * @brief The SpacialNodeComponent class
 */
class O3S_API SpacialNodeComponent : public Component
{
public:

    SpacialNodeComponent();
    virtual ~SpacialNodeComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

    virtual SceneUIElement *sceneUiElement(SceneUIElement::Mode mode) override;
    virtual Panel* panel(Panel::PanelType panelType, Hub *hub) override;

protected:
};

/**
 * @brief The SpacialNodeHub class is a specialized structural node with a transform.
 * @see DummyHub for structural node without transform.
 */
class O3S_API SpacialNodeHub : public StructuralHub
{
public:

    enum NodePolicy
    {
        POLICY_DEFAULT = 0
    };

    enum TransformType
    {
        MATRIX_TRANSFORM = 0,       //!< Common SRT transform
        FPS_TRANSFORM,              //!< First person view transform
        DUAL_QUATERNION_TRANSFORM   //!< Dual quaternion based transform
    };

    explicit SpacialNodeHub(const String &name, Entity *parent = nullptr);
    virtual ~SpacialNodeHub();

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
    // Transformations
    //

    // assume one transform for the moment
    void setPosition(UInt32 transformIndex, const o3d::Vector3f &pos);
    void setRotation(UInt32 transformIndex, const o3d::Vector3f &rot);
    void setRotation(UInt32 transformIndex, const o3d::Quaternion &rot);
    void setScale(UInt32 transformIndex, const o3d::Vector3f &scale);

    UInt32 getNumTransforms() const;
//    UInt32 addTransform(TransformType type);
//    UInt32 insertTransform(TransformType type, UInt32 at);
//    void removeTransform(UInt32 at);

    virtual const Transform* transform(UInt32 transformIndex) const override;

    virtual const o3d::Matrix4& absoluteMatrix(MasterScene *masterScene) const override;

    virtual Bool isSpacialNode() const override;

    //
    // Properties
    //

    // @todo manage children and animations

    /**
     * @brief Add the scene object to the node of the corresponding master scene.
     * @param masterScene Target master scene.
     * @param sceneObject Valid scene object to setup into.
     * @return True if master scene exists, and added into it.
     */
    virtual Bool addChildToScene(MasterScene *masterScene, SceneObject *sceneObject);

    /**
     * @brief Remove the scene object to the node of the corresponding master scene.
     * @param masterScene Target master scene.
     * @param sceneObject Valid scene object to remove from (not delete).
     * @return True if master scene exists, object found and removed from it.
     */
    virtual Bool removeChildFromScene(MasterScene *masterScene, SceneObject *sceneObject);

protected:

    NodePolicy m_nodePolicy;

    std::map<MasterScene*, o3d::Node*> m_instances;

    std::vector<o3d::Transform*> m_transforms;
};

/**
 * @brief The SpacialNodePropertyPanel
 */
class SpacialNodePropertyPanel : public Panel, public EvtHandler
{
    Q_DECLARE_TR_FUNCTIONS(SpacialNodePropertyPanel)

public:

    SpacialNodePropertyPanel(SpacialNodeHub *hub);
    virtual ~SpacialNodePropertyPanel();

    virtual String elementName() const override;

    virtual QWidget* ui() override;

    virtual PanelType panelType() const override;

    virtual void commit() override;
    virtual void update() override;

private:

    LightRef m_ref;
    SpacialNodeHub *m_hub;

    // @todo multi transforms
    Vector3Property *m_position;
    Vector3Property *m_rotation;
    Vector3Property *m_scale;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SPACIALNODEHUB_H

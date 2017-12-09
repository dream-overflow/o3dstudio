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
#include "../workspace/hub.h"

#include <map>

namespace o3d {

class Node;
class SceneObject;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

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
 * @brief The SpacialNodeHub class
 */
class O3S_API SpacialNodeHub : public Hub
{
public:

    enum NodePolicy
    {
        POLICY_DEFAULT = 0
    };

    explicit SpacialNodeHub(const String &name, Entity *parent = nullptr);
    virtual ~SpacialNodeHub();

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

    //
    // Properties
    //

    // @todo manage children, transformations and animations

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
};

/**
 * @brief The SpacialNodePropertyPanel
 */
class SpacialNodePropertyPanel : public Panel
{
    Q_DECLARE_TR_FUNCTIONS(SpacialNodePropertyPanel)

public:

    SpacialNodePropertyPanel(SpacialNodeHub *hub);
    virtual ~SpacialNodePropertyPanel();

    virtual String elementName() const;

    virtual QWidget* ui();

    virtual PanelType panelType() const;

private:

    SpacialNodeHub *m_hub;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SPACIALNODEHUB_H

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

    SceneUIElement *sceneUiElement(SceneUIElement::Mode mode);
    Panel* panel(Panel::PanelType panelType);

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

protected:

    NodePolicy m_nodePolicy;

    std::map<MasterScene*, o3d::Node*> m_instances;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SPACIALNODEHUB_H

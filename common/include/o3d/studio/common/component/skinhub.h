/**
 * @brief Common skin mesh component offering a 3d skinned mesh.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#ifndef _O3DS_COMMON_SKINHUB_H
#define _O3DS_COMMON_SKINHUB_H

#include "component.h"
#include "../workspace/hub.h"

#include <map>

namespace o3d {

class Skin;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

/**
 * @brief The SkinComponent class
 */
class O3S_API SkinComponent : public Component
{
public:

    SkinComponent();
    virtual ~SkinComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

    virtual SceneUIElement* sceneUiElement(SceneUIElement::Mode mode) override;
    virtual Panel* panel(Panel::PanelType panelType, Hub *hub) override;

protected:
};

/**
 * @brief The SkinHub class
 */
class O3S_API SkinHub : public Hub
{
public:

    explicit SkinHub(const String &name, Entity *parent = nullptr);
    virtual ~SkinHub();

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
    // Geometry
    //

    // @todo as mesh

    //
    // Materials
    //

    // @todo as mesh

    //
    // Skeleton
    //

    // @todo

protected:

    std::map<MasterScene*, o3d::Skin*> m_instances;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_SKINHUB_H

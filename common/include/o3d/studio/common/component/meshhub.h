/**
 * @brief Common static mesh component offering a 3d static mesh.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-12-08
 * @details
 */

#ifndef _O3DS_COMMON_MESHHUB_H
#define _O3DS_COMMON_MESHHUB_H

#include "component.h"
#include "../workspace/hub.h"

#include <map>

namespace o3d {

class Mesh;

namespace studio {
namespace common {

class Entity;
class Project;
class Hub;

/**
 * @brief The MeshComponent class
 */
class O3S_API MeshComponent : public Component
{
public:

    MeshComponent();
    virtual ~MeshComponent();

    virtual void setup() override;
    virtual Hub* buildHub(const String &name, Project *project, Entity *parent) override;

    virtual SceneUIElement* sceneUiElement(SceneUIElement::Mode mode) override;
    virtual Panel* panel(Panel::PanelType panelType, Hub *hub) override;

protected:
};

/**
 * @brief The MeshHub class
 */
class O3S_API MeshHub : public Hub
{
public:

    explicit MeshHub(const String &name, Entity *parent = nullptr);
    virtual ~MeshHub();

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

    // @todo temporary but need to used a GeometryResource
    void setVertices(const SmartArrayFloat &v);
    void setNormals(const SmartArrayFloat &v);
    void setUVs(const SmartArrayFloat &v);
    void addIndices(const SmartArrayUInt32 &indices);

    //
    // Materials
    //

    // @todo temporary but need to used a MaterialResource
    void setDiffuseMap(const String &map);

protected:

    std::map<MasterScene*, o3d::Mesh*> m_instances;

    SmartArrayFloat m_vertices;
    SmartArrayFloat m_normals;
    SmartArrayFloat m_uvs;
    SmartArrayUInt32 m_indices;
    String m_diffuseMap;
};

} // namespace common
} // namespace studio
} // namespace o3d

#endif // _O3DS_COMMON_MESHHUB_H

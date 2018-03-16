/**
 * @brief Common spacial node component offering a 3d node for scene graph.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-11
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/hierarchy/hierarchytree.h>
#include <o3d/engine/object/mtransform.h>
#include <o3d/engine/object/ftransform.h>
#include <o3d/engine/object/dqtransform.h>

#include "o3d/studio/common/ui/uiutils.h"
#include "o3d/studio/common/ui/panelbuilder.h"
#include "o3d/studio/common/ui/property/vector3property.h"

#include "o3d/studio/common/component/spacialnodehub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

using namespace o3d::studio::common;


SpacialNodeComponent::SpacialNodeComponent()
{
    m_name = "o3s::common::component::spacialhub";
    m_targetName = "o3s::common::hub::spacialhub";

    m_label = "Spacial node hub";
    m_icon = ":/icons/3d_rotation_black.svg";
}

SpacialNodeComponent::~SpacialNodeComponent()
{

}

void SpacialNodeComponent::setup()
{

}

Hub *SpacialNodeComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    SpacialNodeHub *spacialNodeHub = new SpacialNodeHub(name, parent);
    spacialNodeHub->setTypeRef(m_targetTypeRef);

    return spacialNodeHub;
}

SceneUIElement *SpacialNodeComponent::sceneUiElement(SceneUIElement::Mode /*mode*/)
{
    return nullptr;
}

Panel *SpacialNodeComponent::panel(Panel::PanelType panelType, Hub *hub)
{
    if (!hub) {
        return nullptr;
    }

    if (hub->ref().strong().typeName() != m_targetName) {
        // not a spacial node hub
        return nullptr;
    }

    SpacialNodeHub *lhub = static_cast<SpacialNodeHub*>(hub);

    if (panelType == Panel::PANEL_PROPERTY) {
        return new SpacialNodePropertyPanel(lhub);
    }

    return nullptr;
}

SpacialNodeHub::SpacialNodeHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_nodePolicy(POLICY_DEFAULT),
    m_instances()
{
    // with at least one default MTransform
    MTransform *mainTransform = new MTransform;
    m_transforms.push_back(mainTransform);
}

SpacialNodeHub::~SpacialNodeHub()
{
    O3D_ASSERT(m_instances.empty());

    for (auto it = m_transforms.begin(); it != m_transforms.end(); ++it) {
        delete(*it);
    }
}

void SpacialNodeHub::create()
{
    Hub::create();

    // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void SpacialNodeHub::destroy()
{
    Hub::destroy();

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

void SpacialNodeHub::update()
{
     for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::SYNC);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool SpacialNodeHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool SpacialNodeHub::load()
{
    return Hub::load();
}

o3d::Bool SpacialNodeHub::save()
{
    return Hub::save();
}

o3d::Bool SpacialNodeHub::exists() const
{
    return Entity::exists();
}

o3d::Bool SpacialNodeHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    return True;
}

o3d::Bool SpacialNodeHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    return True;
}

void SpacialNodeHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Node *node = new o3d::Node(masterScene->scene());
    node->setName(m_name);

    // add the primary transform
    node->addTransform(new MTransform);

    // determine where to put this node
    if (isParentSpacialNode()) {
        // the parent hub is also a spacial node so take its node as parent
        SpacialNodeHub *parentHub = static_cast<SpacialNodeHub*>(parent());

        auto it = parentHub->m_instances.find(masterScene);
        if (it != m_instances.end()) {
            o3d::Node *parentNode = it->second;
            parentNode->addSonLast(node);
        } else {
            // parent node not found
            masterScene->scene()->getHierarchyTree()->getRootNode()->addSonLast(node);
        }
    } else {
        // not spacial hub as parent
        masterScene->scene()->getHierarchyTree()->getRootNode()->addSonLast(node);
    }

    m_instances[masterScene] = node;

    // scene object id is as the base of the pickable color id
    project()->addPickable((UInt32)node->getId(), this);

    O3D_MESSAGE("SpacialNodeHub created into scene");
}

void SpacialNodeHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Node *node = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)node->getId());

        node->getParent()->deleteChild(node);

        O3D_MESSAGE("SpacialNodeHub deleted from scene");
    }
}

void SpacialNodeHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Node *node = it->second;

        // hub => o3d
        node->setName(m_name);

        // o3d => hub
        o3d::Transform *nodeTransform = node->getTransform();
        if (nodeTransform) {
            o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transforms.front());

            nodeTransform->setPosition(mainTransform->getPosition());
            nodeTransform->setRotation(mainTransform->getRotation());
            nodeTransform->setScale(mainTransform->getScale());
        }

        O3D_MESSAGE("SpacialNodeHub synced into scene");
    }
}

void SpacialNodeHub::setPosition(o3d::UInt32 transformIndex, const o3d::Vector3f &pos)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transforms.front());
    mainTransform->setPosition(pos);

    setDirty();
}

void SpacialNodeHub::setRotation(o3d::UInt32 transformIndex, const o3d::Vector3f &rot)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transforms.front());
    o3d::Quaternion q;
    q.fromEuler(rot);

    mainTransform->setRotation(q);

    setDirty();
}

void SpacialNodeHub::setRotation(o3d::UInt32 transformIndex, const o3d::Quaternion &rot)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transforms.front());
    mainTransform->setRotation(rot);

    setDirty();
}

void SpacialNodeHub::setScale(o3d::UInt32 transformIndex, const o3d::Vector3f &scale)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transforms.front());
    mainTransform->setScale(scale);

    setDirty();
}

o3d::UInt32 SpacialNodeHub::getNumTransforms() const
{
    return (UInt32)m_transforms.size();
}

const o3d::Transform &SpacialNodeHub::transform(o3d::UInt32 transformIndex) const
{
    if (transformIndex < m_transforms.size()) {
        return *m_transforms[transformIndex];
    } else {
        O3D_ERROR(E_IndexOutOfRange("Transform index"));
    }
}

const o3d::Matrix4 &SpacialNodeHub::absoluteMatrix(MasterScene *masterScene) const
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        return it->second->getAbsoluteMatrix();
    }

    return Matrix4::getIdentity();
}

o3d::Bool SpacialNodeHub::isSpacialNode() const
{
    return True;
}

o3d::Bool SpacialNodeHub::addChildToScene(MasterScene *masterScene, o3d::SceneObject *sceneObject)
{
    if (!masterScene || !sceneObject) {
        return False;
    }

    auto it = m_instances.find(masterScene);
    if (it == m_instances.end()) {
        return False;
    }

    o3d::Node *node = it->second;
    node->addSonLast(sceneObject);

    return True;
}

o3d::Bool SpacialNodeHub::removeChildFromScene(MasterScene *masterScene, o3d::SceneObject *sceneObject)
{
    if (!masterScene || !sceneObject) {
        return False;
    }

    auto it = m_instances.find(masterScene);
    if (it == m_instances.end()) {
        return False;
    }

    o3d::Node *node = it->second;
    if (node->hasSon(sceneObject)) {
        node->removeSon(sceneObject);
    }

    return True;
}

SpacialNodePropertyPanel::SpacialNodePropertyPanel(SpacialNodeHub *hub) :
    m_hub(hub)
{
    O3D_ASSERT(m_hub);
    O3D_ASSERT(m_hub->exists());

    if (m_hub) {
        m_ref = m_hub->ref().light();
    }
}

SpacialNodePropertyPanel::~SpacialNodePropertyPanel()
{

}

o3d::String SpacialNodePropertyPanel::elementName() const
{
    return "o3s::common::component::panel::property::spacialnode";
}

QWidget *SpacialNodePropertyPanel::ui()
{
    PanelBuilder pb(this, fromQString(tr("Spacial node hub")));

    m_position = new Vector3Property(this, "position", fromQString(tr("Position")));
    pb.addPanelProperty(m_position);

    m_rotation = new Vector3Property(this, "rotation", fromQString(tr("Rotation")));
    pb.addPanelProperty(m_rotation);

    m_scale = new Vector3Property(this, "scale", fromQString(tr("Scale")));
    pb.addPanelProperty(m_scale);

    // initial scale
    m_scale->setValue(Vector3f(1,1,1));

    return pb.ui();
}

Panel::PanelType SpacialNodePropertyPanel::panelType() const
{
    return PANEL_PROPERTY;
}

void SpacialNodePropertyPanel::commit()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace->hub(m_ref) == m_hub) {
        m_hub->setPosition(0, m_position->value());
        m_hub->setRotation(0, m_rotation->value());
        m_hub->setScale(0, m_scale->value());

        m_hub->update();
    }
}

void SpacialNodePropertyPanel::update()
{
    common::Workspace* workspace = common::Application::instance()->workspaces().current();
    if (workspace->hub(m_ref) == m_hub) {
        Vector3 euler;
        m_hub->transform(0).getRotation().toEuler(euler);
        // @todo range clamp...

        m_position->setValue(m_hub->transform(0).getPosition());
        m_rotation->setValue(euler);
        m_scale->setValue(m_hub->transform(0).getScale());
    }
}

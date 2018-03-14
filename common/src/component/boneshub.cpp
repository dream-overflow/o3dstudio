/**
 * @brief Common skeleton bones component offering a bones for a skeleton.
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-02-17
 * @details
 */

#include <o3d/engine/scene/scene.h>
#include <o3d/engine/hierarchy/hierarchytree.h>
#include <o3d/engine/object/mtransform.h>
#include <o3d/engine/object/ftransform.h>
#include <o3d/engine/object/dqtransform.h>
#include <o3d/engine/object/bones.h>
#include <o3d/engine/object/skeleton.h>

#include "o3d/studio/common/ui/uiutils.h"
#include "o3d/studio/common/ui/panelbuilder.h"
#include "o3d/studio/common/ui/property/floatproperty.h"
#include "o3d/studio/common/ui/property/boolproperty.h"
#include "o3d/studio/common/ui/property/vector3property.h"
#include "o3d/studio/common/ui/property/floatproperty.h"

#include "o3d/studio/common/component/boneshub.h"
#include "o3d/studio/common/workspace/project.h"
#include "o3d/studio/common/workspace/masterscene.h"
#include "o3d/studio/common/workspace/scenecommand.h"

using namespace o3d::studio::common;


BonesComponent::BonesComponent()
{
    m_name = "o3s::common::component::boneshub";
    m_targetName = "o3s::common::hub::boneshub";

    m_label = "Bones hub";
    m_icon = ":/icons/merge_type_black.svg";
}

BonesComponent::~BonesComponent()
{

}

void BonesComponent::setup()
{

}

Hub *BonesComponent::buildHub(const String &name, Project *project, Entity *parent)
{
    if (!project) {
        return nullptr;
    }

    if (!parent) {
        return nullptr;
    }

    BonesHub *bonesHub = new BonesHub(name, parent);
    bonesHub->setTypeRef(m_targetTypeRef);

    return bonesHub;
}

SceneUIElement *BonesComponent::sceneUiElement(SceneUIElement::Mode /*mode*/)
{
    return nullptr;
}

Panel *BonesComponent::panel(Panel::PanelType panelType, Hub *hub)
{
    if (!hub) {
        return nullptr;
    }

    if (hub->ref().strong().typeName() != m_targetName) {
        // not a bones hub
        return nullptr;
    }

    BonesHub *lhub = static_cast<BonesHub*>(hub);

    if (panelType == Panel::PANEL_PROPERTY) {
        return new BonesPropertyPanel(lhub);
    }

    return nullptr;
}

BonesHub::BonesHub(const String &name, Entity *parent) :
    Hub(name, parent),
    m_instances(),
    m_transform(nullptr)
{
    // with at least one default MTransform
    m_transform = new MTransform;
}

BonesHub::~BonesHub()
{
    O3D_ASSERT(m_instances.empty());

    deletePtr(m_transform);
}

void BonesHub::create()
{
    Hub::create();

    // sync with the master scene
    SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::CREATE);
    project()->masterScene()->addCommand(sceneCommand);
}

void BonesHub::destroy()
{
    Hub::destroy();

    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        // sync with master scenes
        SceneCommand *sceneCommand = new SceneHubCommand(this, SceneHubCommand::DELETE);
        it->first->addCommand(sceneCommand);
    }
}

o3d::Bool BonesHub::deletable() const
{
    // deletable when no more attached to master scene
    return m_instances.empty();
}

o3d::Bool BonesHub::load()
{
    return Hub::load();
}

o3d::Bool BonesHub::save()
{
    return Hub::save();
}

o3d::Bool BonesHub::exists() const
{
    return Entity::exists();
}

o3d::Bool BonesHub::serializeContent(OutStream &stream) const
{
    if (!Hub::serializeContent(stream)) {
        return False;
    }

    // @todo issue during save transform
    stream << m_radius
           << m_length
           << m_isEndEffector
           << *m_transform;

    return True;
}

o3d::Bool BonesHub::deserializeContent(InStream &stream)
{
    if (!Hub::deserializeContent(stream)) {
        return False;
    }

    stream >> m_radius
           >> m_length
           >> m_isEndEffector
           >> *m_transform;

    return True;
}

void BonesHub::createToScene(MasterScene *masterScene)
{
    if (!masterScene) {
        return;
    }

    o3d::Bones *bones = new o3d::Bones(masterScene->scene());
    bones->setName(m_name);

    // determine where to put this node
    if (parent() && parent()->typeRef() == typeRef()) {
        // the parent hub is also a bones so take its node as parent
        BonesHub *parentHub = static_cast<BonesHub*>(parent());

        // @todo or parent is a SkeletonHub or Resource ??
        auto it = parentHub->m_instances.find(masterScene);
        if (it != m_instances.end()) {
            o3d::Bones *parentNode = it->second;
            parentNode->addSonLast(bones);
        } else {
            // parent node not found
            masterScene->scene()->getHierarchyTree()->getRootNode()->addSonLast(bones);
        }
    } else {
        // not spacial hub as parent
        masterScene->scene()->getHierarchyTree()->getRootNode()->addSonLast(bones);
    }

    m_instances[masterScene] = bones;

    // scene object id is as the base of the pickable color id
    project()->addPickable((UInt32)bones->getId(), this);

    O3D_MESSAGE("BonesHub created into scene");
}

void BonesHub::removeFromScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Bones *bones = it->second;
        m_instances.erase(it);

        // scene object id is as the base of the pickable color id
        project()->removePickable((UInt32)bones->getId());

        bones->getParent()->deleteChild(bones);

        O3D_MESSAGE("BonesHub deleted from scene");
    }
}

void BonesHub::syncWithScene(MasterScene *masterScene)
{
    auto it = m_instances.find(masterScene);
    if (it != m_instances.end()) {
        o3d::Node *node = it->second;

        // hub => o3d
        node->setName(m_name);

        // o3d => hub
        o3d::Transform *nodeTransform = node->getTransform();
        if (nodeTransform) {
            o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transform);

            nodeTransform->setPosition(mainTransform->getPosition());
            nodeTransform->setRotation(mainTransform->getRotation());
        }

        O3D_MESSAGE("SpacialNodeHub synced into scene");
    }
}

void BonesHub::setPosition(const o3d::Vector3f &pos)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transform);
    mainTransform->setPosition(pos);
}

void BonesHub::setRotation(const o3d::Vector3f &rot)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transform);
    o3d::Quaternion q;
    q.fromEuler(rot);

    mainTransform->setRotation(q);
}

void BonesHub::setScale(const o3d::Vector3f &scale)
{
    o3d::MTransform *mainTransform = static_cast<o3d::MTransform*>(m_transform);
    mainTransform->setScale(scale);
}

o3d::Bool BonesHub::addChildToScene(MasterScene *masterScene, o3d::SceneObject *sceneObject)
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

o3d::Bool BonesHub::removeChildFromScene(MasterScene *masterScene, o3d::SceneObject *sceneObject)
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

BonesPropertyPanel::BonesPropertyPanel(BonesHub *hub) :
    m_hub(hub),
    m_isEndEffector(nullptr),
    m_length(nullptr),
    m_radius(nullptr),
    m_position(nullptr),
    m_rotation(nullptr)
{

}

BonesPropertyPanel::~BonesPropertyPanel()
{

}

o3d::String BonesPropertyPanel::elementName() const
{
    return "o3s::common::component::panel::property::bones";
}

QWidget *BonesPropertyPanel::ui()
{
    PanelBuilder pb(this, fromQString(tr("Bones hub")));

    m_position = new Vector3Property(this, "position", fromQString(tr("Position")));
    pb.addPanelProperty(m_position);

    m_rotation = new Vector3Property(this, "rotation", fromQString(tr("Rotation")));
    pb.addPanelProperty(m_rotation);

    m_radius = new FloatProperty(this, "radius", fromQString(tr("Radius")));
    pb.addPanelProperty(m_radius);

    m_length = new FloatProperty(this, "length", fromQString(tr("Length")));
    pb.addPanelProperty(m_length);

    m_isEndEffector = new BoolProperty(this, "isendeffector", fromQString(tr("End effector")));
    pb.addPanelProperty(m_isEndEffector);

    return pb.ui();
}

Panel::PanelType BonesPropertyPanel::panelType() const
{
    return PANEL_PROPERTY;
}

void BonesPropertyPanel::commit()
{
    if (m_hub) {
        m_hub->setPosition(m_position->value());
        m_hub->setRotation(m_rotation->value());
        // m_hub->setRadius @todo
    }
}

void BonesPropertyPanel::update()
{

}

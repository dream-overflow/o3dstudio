/**
 * @brief Application selection item.
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-0
 * @details
 */

#include "o3d/studio/common/workspace/selectionitem.h"
#include "o3d/studio/common/workspace/entity.h"

#include "o3d/studio/common/application.h"
#include "o3d/studio/common/workspace/workspace.h"
#include "o3d/studio/common/workspace/workspacemanager.h"

using namespace o3d::studio::common;

SelectionItem::SelectionItem(Entity *entity)
{
    O3D_ASSERT(entity != nullptr);

    if (entity) {
        if (entity->parent()) {
            m_parentRef = entity->parent()->ref().light();
        }

        entity->setSelected(True);

        m_ref = entity->ref().light();
    }
}

SelectionItem::SelectionItem(const LightRef &ref) :
    m_parentRef(),
    m_ref(ref)
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();

    Entity *entity = workspace->entity(m_ref);
    if (entity) {
        entity->setSelected(False);
    }
}

SelectionItem::SelectionItem(const LightRef &ref, const LightRef &parentRef) :
    m_parentRef(parentRef),
    m_ref(ref)
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();

    Entity *entity = workspace->entity(m_ref);
    if (entity) {
        entity->setSelected(True);
    }
}

SelectionItem::~SelectionItem()
{
    common::Workspace *workspace = common::Application::instance()->workspaces().current();

    Entity *entity = workspace->entity(m_ref);
    if (entity) {
        entity->setSelected(False);
    }
}

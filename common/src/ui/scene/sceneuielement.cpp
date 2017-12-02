/**
 * @brief Common scene UI element base class
 * @copyright Copyright (C) 2017 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2017-11-30
 * @details
 */

#include "o3d/studio/common/ui/scene/sceneuielement.h"

using namespace o3d::studio::common;

SceneUIElement::SceneUIElement(BaseObject *parent,
                               SceneUIElement::UIType uiType,
                               DrawStep drawStep,
                               Bool directDraw) :
    BaseObject(parent),
    m_uiType(uiType),
    m_drawStep(drawStep),
    m_directDraw(directDraw)
{

}

SceneUIElement::~SceneUIElement()
{

}

o3d::Bool SceneUIElement::isDirectDraw() const
{
    return m_directDraw;
}

SceneUIElement::DrawStep SceneUIElement::drawStep() const
{
    return m_drawStep;
}
